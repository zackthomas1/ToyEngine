#include <toy_engine.h>

void DrawSceneNodeTree(ToyEngine::SceneNode* node) {
	if(!node) return; 
	if (ImGui::TreeNode(node->GetName().c_str())) {
		for (const auto& child : node->GetChildren()) {
			DrawSceneNodeTree(child.get());
		}
		ImGui::TreePop();
	}
}

class Scene : public ToyEngine::Layer
{
public:
	Scene()
	{
		TY_INFO("Compiling shaders...");
		m_shader_lib = ToyEngine::MakeRef<ToyEngine::ShaderLibrary>();
		ToyEngine::Ref<ToyEngine::Shader> flatShader	= ToyEngine::Shader::Create("flat_color", "../assets/shaders/flat_color.vs", "../assets/shaders/flat_color.fs");
		ToyEngine::Ref<ToyEngine::Shader> textureShader = ToyEngine::Shader::Create("flat_texture", "../assets/shaders/flat_texture.vs", "../assets/shaders/flat_texture.fs");
		ToyEngine::Ref<ToyEngine::Shader> phongShader	= ToyEngine::Shader::Create("phong", "../assets/shaders/phong.vs", "../assets/shaders/phong.fs");
		ToyEngine::Ref<ToyEngine::Shader> skyboxShader	= ToyEngine::Shader::Create("skybox", "../assets/shaders/skybox.vs", "../assets/shaders/skybox.fs");
		ToyEngine::Ref<ToyEngine::Shader> postfxShader	= ToyEngine::Shader::Create("postfx", "../assets/shaders/post_process.vs", "../assets/shaders/post_process.fs");

		m_shader_lib->Add(flatShader);
		m_shader_lib->Add(textureShader);
		m_shader_lib->Add(phongShader);
		m_shader_lib->Add(skyboxShader);
		m_shader_lib->Add(postfxShader);

		ToyEngine::Renderer::GetUniformManager().BindUniformBlockToShader(flatShader, "ViewProjectMats");
		ToyEngine::Renderer::GetUniformManager().BindUniformBlockToShader(textureShader, "ViewProjectMats");
		ToyEngine::Renderer::GetUniformManager().BindUniformBlockToShader(phongShader, "ViewProjectMats");
		ToyEngine::Renderer::GetUniformManager().BindUniformBlockToShader(skyboxShader, "ViewProjectMats");

		ToyEngine::Renderer::GetUniformManager().BindUniformBlockToShader(phongShader, "LightBlock");
	}

	virtual void OnAttach()
	{
		TY_INFO("Initializing Camera...");
		m_camera = ToyEngine::MakeScope<ToyEngine::Camera>(ToyEngine::eCameraType::kFlyCamera);

		TY_INFO("Creating lights...");
		m_light_block = ToyEngine::MakeScope<ToyEngine::LightBlock>();
		m_light_block->m_num_lights = 5;
		m_light_block->m_lights[0].m_type = int(ToyEngine::eLightType::kDirectional);
		m_light_block->m_lights[1].m_type = int(ToyEngine::eLightType::kPoint);
		m_light_block->m_lights[2].m_type = int(ToyEngine::eLightType::kPoint);
		m_light_block->m_lights[3].m_type = int(ToyEngine::eLightType::kPoint);
		m_light_block->m_lights[4].m_type = int(ToyEngine::eLightType::kSpot);
		m_light_block->m_lights[0].m_enabled = true;
		m_light_block->m_lights[1].m_enabled = true;
		m_light_block->m_lights[2].m_enabled = true;
		m_light_block->m_lights[3].m_enabled = true;
		m_light_block->m_lights[4].m_enabled = true;

		// Create scene 
		TY_INFO("Create scene...");
		m_scene_graph = ToyEngine::MakeScope<ToyEngine::SceneNode>("root");

		ToyEngine::Ref<ToyEngine::Model> backpack		= (ToyEngine::Model::Create("../assets/models/backpack/backpack.obj", true));
		ToyEngine::Ref<ToyEngine::Model> cyborg			= (ToyEngine::Model::Create("../assets/models/cyborg/cyborg.obj", false));
		ToyEngine::Ref<ToyEngine::Shader> phongShader	= m_shader_lib->Get("phong");
		backpack->m_shader = phongShader;
		cyborg->m_shader = phongShader;
		m_scene_graph->AddChild(ToyEngine::MakeScope<ToyEngine::SceneNode>("backepack_model", backpack));
		m_scene_graph->AddChild(ToyEngine::MakeScope<ToyEngine::SceneNode>("cyborg_model", cyborg));
	
		ToyEngine::Array<std::string, 6> skybox_files = {
			"../assets/cubemaps/skybox/right.jpg",	// +X (right)
			"../assets/cubemaps/skybox/left.jpg",	// -X (left)
			"../assets/cubemaps/skybox/top.jpg",	// +Y (top)
			"../assets/cubemaps/skybox/bottom.jpg",	// -Y (bottom)
			"../assets/cubemaps/skybox/front.jpg",	// +Z (front)
			"../assets/cubemaps/skybox/back.jpg",	// -Z (back)
		};
		ToyEngine::Ref<ToyEngine::TextureCube> sky_texture = ToyEngine::TextureCube::Create(skybox_files);
		ToyEngine::Ref<ToyEngine::Skybox> skybox = ToyEngine::MakeRef<ToyEngine::Skybox>(sky_texture, m_shader_lib->Get("skybox"));
		m_scene_graph->AddChild(ToyEngine::MakeScope<ToyEngine::SceneNode>("skybox", skybox));

		for (auto& mesh : backpack->m_meshes) {
			mesh->m_material->SetEnvironmentMap(sky_texture);
		}
		for (auto& mesh : cyborg->m_meshes) {
			mesh->m_material->SetEnvironmentMap(sky_texture);
		}

		// Create  Framebuffer
		TY_INFO("Create Framebuffer...");
		ToyEngine::FrameBufferProps fb_props;
		fb_props.width = ToyEngine::Application::AccessWindow().GetWidth(); 
		fb_props.height = ToyEngine::Application::AccessWindow().GetHeight();
		m_frame_buffer = ToyEngine::FrameBuffer::Create(fb_props);

		m_quad_vertex_array = ToyEngine::VertexArray::Create();
		ToyEngine::Ref<ToyEngine::VertexBuffer>quad_vertex_buffer = ToyEngine::VertexBuffer::Create(ToyEngine::TextureQuadPrim::m_vertices.data(), 
																								sizeof(ToyEngine::TextureQuadPrim::m_vertices));
		quad_vertex_buffer->SetLayout(ToyEngine::TextureQuadPrim::m_layout);
		m_quad_vertex_array->AddBuffer(quad_vertex_buffer);
		ToyEngine::Ref<ToyEngine::IndexBuffer>quad_index_buffer = ToyEngine::IndexBuffer::Create(ToyEngine::TextureQuadPrim::m_indices.data(),
																							 ToyEngine::TextureQuadPrim::m_indices.size());
		m_quad_vertex_array->SetIndexBuffer(quad_index_buffer);
}

	virtual void OnDetach() {}

	virtual void Update(const ToyEngine::TimeStep& time_step)
	{
		//Keyboard input polling
		float delta_time = time_step.GetTimeDelta();
		ToyEngine::InputPoll& input = ToyEngine::Locator::InputPollService();
		if (input.Key(ToyEngine::eKeyCode::kKeyW) != ToyEngine::eKeyState::kRelease)
			m_camera->UpdatePosition(ToyEngine::eCameraMovement::kForward, delta_time);
		if (input.Key(ToyEngine::eKeyCode::kKeyS) != ToyEngine::eKeyState::kRelease)
			m_camera->UpdatePosition(ToyEngine::eCameraMovement::kBackward, delta_time);
		if (input.Key(ToyEngine::eKeyCode::kKeyA) != ToyEngine::eKeyState::kRelease)
			m_camera->UpdatePosition(ToyEngine::eCameraMovement::kLeft, delta_time);
		if (input.Key(ToyEngine::eKeyCode::kKeyD) != ToyEngine::eKeyState::kRelease)
			m_camera->UpdatePosition(ToyEngine::eCameraMovement::kRight, delta_time);
		if (input.Key(ToyEngine::eKeyCode::kKeyE) != ToyEngine::eKeyState::kRelease)
			m_camera->UpdatePosition(ToyEngine::eCameraMovement::kUp, delta_time);
		if (input.Key(ToyEngine::eKeyCode::kKeyQ) != ToyEngine::eKeyState::kRelease)
			m_camera->UpdatePosition(ToyEngine::eCameraMovement::kDown, delta_time);

		// Update lights
		m_light_block->m_lights[0].m_direction		= glm::vec4(m_directional_light_dir,0.0f);
		m_light_block->m_lights[0].m_value			= glm::vec4(m_directional_light_color, 0.0f);
		m_light_block->m_lights[1].m_position		= glm::vec4(m_point_light_position_1, 0.0f);
		m_light_block->m_lights[1].m_value			= glm::vec4(m_point_light_color_1, 0.0f);
		m_light_block->m_lights[2].m_position		= glm::vec4(m_point_light_position_2, 0.0f);
		m_light_block->m_lights[2].m_value			= glm::vec4(m_point_light_color_2, 0.0f);
		m_light_block->m_lights[3].m_position		= glm::vec4(m_point_light_position_3, 0.0f);
		m_light_block->m_lights[3].m_value			= glm::vec4(m_point_light_color_3, 0.0f);
		m_light_block->m_lights[4].m_position		= glm::vec4(m_camera->position(), 0.0f);
		m_light_block->m_lights[4].m_spotDirection	= glm::vec4(m_camera->front(), 0.0f);
		m_light_block->m_lights[4].m_value			= glm::vec4(m_spot_light_color, 0.0f);

		// update model material properties
		if (m_scene_graph->GetChildren().size() >= 2) {
			auto& backpack_node = m_scene_graph->GetChildren()[0];
			auto& cyborg_node = m_scene_graph->GetChildren()[1];

			if (auto& backpack_model = std::dynamic_pointer_cast<ToyEngine::Model>(backpack_node->GetEntity())) {
				for (auto& mesh : backpack_model->m_meshes) {
					mesh->m_material->SetRoughness(m_roughness);
					mesh->m_material->SetMetallic(m_metallic);
					mesh->m_material->SetTransmission(m_transmission);
					mesh->m_material->SetRefractiveIndex(m_refractive_index);
				}
			}
			if (auto& cyborg_model = std::dynamic_pointer_cast<ToyEngine::Model>(cyborg_node->GetEntity())) {
				for (auto& mesh : cyborg_model->m_meshes) {
					mesh->m_material->SetRoughness(m_roughness);
					mesh->m_material->SetMetallic(m_metallic);
					mesh->m_material->SetTransmission(m_transmission);
					mesh->m_material->SetRefractiveIndex(m_refractive_index);
				}
			}
		}

		// update model transforms
		m_scene_graph->SetLocalTransform(glm::translate(glm::mat4(1.0f), m_translate));
		m_scene_graph->GetChildren()[1]->SetLocalTransform(
			glm::rotate(glm::translate(glm::mat4(1.0f), m_translate_cyborg),
				glm::radians(m_rotation_degree), glm::vec3(0.0f, 1.0f, 0.0f)));

		// Check if window was resized and recreate framebuffer if needed
		uint32_t current_width = ToyEngine::Application::AccessWindow().GetWidth();
		uint32_t current_height = ToyEngine::Application::AccessWindow().GetHeight();

		if (current_width != m_frame_buffer->GetWidth() || current_height != m_frame_buffer->GetHeight()) {
			m_frame_buffer->Resize(current_width, current_height);
		}

		// Draw Scene
		m_frame_buffer->Bind();
		ToyEngine::Renderer::BeginScene(m_camera.get(), m_light_block.get());
		ToyEngine::Renderer::Submit(m_scene_graph.get());
		ToyEngine::Renderer::EndScene();
		m_frame_buffer->Unbind();

		// Draw quad with post-processing 
		ToyEngine::Ref<ToyEngine::Shader> postfxShader = m_shader_lib->Get("postfx");
		postfxShader->Use();
		postfxShader->SetInt("screenTexture", 0);
		bool depth_test;
		ToyEngine::RenderCommand::GetBooleanv(ToyEngine::eParamType::kDEPTH_TEST, &depth_test);
		ToyEngine::RenderCommand::Disable(ToyEngine::eParamType::kDEPTH_TEST);
		ToyEngine::RenderCommand::ClearSetBackground();
		ToyEngine::RenderCommand::BindTexture(ToyEngine::eSamplerType::kTexture2D, m_frame_buffer->GetColorAttachment());
		ToyEngine::Renderer::Submit(m_quad_vertex_array.get());
		if (depth_test) ToyEngine::RenderCommand::Enable(ToyEngine::eParamType::kDEPTH_TEST);
	}

	virtual void OnImGuiRender() 
	{
		// Define the GUI windows
		// ------------------------------
		ImGuiIO& io_ = ImGui::GetIO(); (void)io_;

		// Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! 
		// You can browse its code to learn more about Dear ImGui!).
		ImGui::ShowDemoWindow();

		ImGui::Begin("Scene Window"); 
		uint32_t color_attachment = m_frame_buffer->GetColorAttachment();
		uint32_t width = m_frame_buffer->GetWidth();
		uint32_t height = m_frame_buffer->GetHeight();
		ImGui::Image(color_attachment,
			ImVec2(width, height),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();

		// Show simple window
		ImGui::Begin("Controls");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io_.Framerate, io_.Framerate);
		if (ImGui::CollapsingHeader("Camera Controls", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Camera FOV: %d", static_cast<int>(m_camera->fov()));

			// Camera type selection widget
			static int camera_type = static_cast<int>(m_camera->GetCameraType());
			const char* camera_types[] = { "Perspective (Fly Camera)", "Orthographic" };
			if (ImGui::Combo("Camera Type", &camera_type, camera_types, IM_ARRAYSIZE(camera_types)))
			{
				// 0: Perspective (Fly Camera), 1: Orthographic
				if (camera_type == 0 && m_camera->GetCameraType() != ToyEngine::eCameraType::kFlyCamera)
				{
					m_camera->SetCameraType(ToyEngine::eCameraType::kFlyCamera);
				}
				else if (camera_type == 1 && m_camera->GetCameraType() != ToyEngine::eCameraType::kOrthographicCamera)
				{
					m_camera->SetCameraType(ToyEngine::eCameraType::kOrthographicCamera);
				}
			}
		}
		if (ImGui::CollapsingHeader("Scene Controls", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::DragFloat3("Translate Root##TranslateRoot", glm::value_ptr(m_translate), 0.1f, -10.0f, 10.0f, "%.1f");
			ImGui::DragFloat3("Translate Cyborg##TranslateCyborg", glm::value_ptr(m_translate_cyborg), 0.1f, -10.0f, 10.0f, "%.1f");
			ImGui::DragFloat("Rotate Cyborg##RotateCyborg", &m_rotation_degree, 0.1f, 0.0f, 360.0f, "%.1f", ImGuiSliderFlags_WrapAround);
		}
		
		if (ImGui::CollapsingHeader("Material Controls", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::DragFloat("Roughness##MaterialRoughness", &m_roughness, 0.1f, 0.1f, 256.0f, "%.1f");
			ImGui::DragFloat("Metallic##MaterialMetallic", &m_metallic, 0.01f, 0.00f, 1.00f, "%.2f");
			ImGui::DragFloat("Transmission##MaterialMetallic", &m_transmission, 0.01f, 0.00f, 1.00f, "%.2f");
			ImGui::DragFloat("IOR##MaterialIOR", &m_refractive_index, 0.1f, 0.1f, 100.0f, "%.2f");
		}

		if (ImGui::CollapsingHeader("Light Controls", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Directional Light");
			ImGui::ColorEdit3("Color##DirectionalLightColor", glm::value_ptr(m_directional_light_color), ImGuiColorEditFlags_Float);
			ImGui::DragFloat3("Direction##DirectionalLightDirection", glm::value_ptr(m_directional_light_dir), 0.01f, -1.0f, 1.0f, "%.2f");

			ImGui::Text("Point Light 1");
			ImGui::ColorEdit3("Color##PointLight1Color", glm::value_ptr(m_point_light_color_1), ImGuiColorEditFlags_Float);
			ImGui::DragFloat3("Position##PointLight1Position", glm::value_ptr(m_point_light_position_1), 0.01f, 0.0f, 0.0f, "%.2f");

			ImGui::Text("Point Light 2");
			ImGui::ColorEdit3("Color##PointLight2Color", glm::value_ptr(m_point_light_color_2), ImGuiColorEditFlags_Float);
			ImGui::DragFloat3("Position##PointLight2Position", glm::value_ptr(m_point_light_position_2), 0.01f, 0.0f, 0.0f, "%.2f");

			ImGui::Text("Point Light 3");
			ImGui::ColorEdit3("Color##PointLight3Color", glm::value_ptr(m_point_light_color_3), ImGuiColorEditFlags_Float);
			ImGui::DragFloat3("Position##PointLight3Position", glm::value_ptr(m_point_light_position_3), 0.01f, 0.0f, 0.0f, "%.2f");

			ImGui::Text("Spot Light");
			ImGui::ColorEdit3("Color##SpotLightColor", glm::value_ptr(m_spot_light_color), ImGuiColorEditFlags_Float);
		}
		ImGui::End();

		ImGui::Begin("Scene Graph");
		DrawSceneNodeTree(m_scene_graph.get());
		ImGui::End();
	}

	virtual void OnEvent(ToyEngine::Event& e)
	{
		// Set flag indiciating that event has been handled by current layer
		e.SetEventHandled(true);
		if (ToyEngine::EventVerticalScroll* event = dynamic_cast<ToyEngine::EventVerticalScroll*>(&e)) {
			m_camera->UpdateFOV(event->GetYOffset());
		}

		if (ToyEngine::EventCursorPos* event = dynamic_cast<ToyEngine::EventCursorPos*>(&e))
		{
			//TY_CORE_INFO("EventCursorPos: x_offset-{} y_offset-{}", event->GetXOffset(), event->GetYOffset());
			m_camera->UpdateLookDirection(static_cast<float>(event->GetXOffset()), static_cast<float>(event->GetYOffset()));
		}
	}
public: 
	ToyEngine::Ref<ToyEngine::ShaderLibrary> m_shader_lib;
	ToyEngine::Scope<ToyEngine::Camera> m_camera;
	ToyEngine::Ref<ToyEngine::FrameBuffer> m_frame_buffer; 
	ToyEngine::Ref<ToyEngine::VertexArray> m_quad_vertex_array;

	// Model control parameters
	ToyEngine::Scope<ToyEngine::SceneNode> m_scene_graph;
	float m_rotation_degree = 0;
	glm::vec3 m_translate = glm::vec3(0.0f), m_translate_cyborg = glm::vec3(0.0f);
	float m_roughness = 32.0f, m_metallic = 0.0f, m_transmission = 0.0f, m_refractive_index = 1.52f;

	// Light control parameters
	ToyEngine::Scope<ToyEngine::LightBlock> m_light_block;
	glm::vec3 m_directional_light_color = glm::vec3(0.2), m_directional_light_dir = glm::vec3(0.0, 0.0, -1.0);
	glm::vec3 m_point_light_color_1 = glm::vec3(1.0, 0.0, 0.0), m_point_light_color_2 = glm::vec3(0.0, 1.0, 0.0), m_point_light_color_3 = glm::vec3(0.0, 0.0, 1.0);
	glm::vec3 m_point_light_position_1 = glm::vec3(1.0, 0.0, 1.0), m_point_light_position_2 = glm::vec3(0.0, 1.0, 1.0), m_point_light_position_3 = glm::vec3(-1.0, 0.0, 1.0);
	glm::vec3 m_spot_light_color = glm::vec3(1.0, 1.0, 0.0);
};

class Editor : public ToyEngine::Application
{
public:
	Editor()
	{
		TY_INFO("Initialize application");
		PushLayer(new Scene());
	}
	~Editor() { }
};

ToyEngine::Application* ToyEngine::CreateApplication()
{
	return new Editor();
}
