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
		m_shader_lib = ToyEngine::MakeScope<ToyEngine::ShaderLibrary>();
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
		TY_INFO("Create Camera...");
		ToyEngine::CameraControllerProps camera_control_props;
		camera_control_props.type = ToyEngine::eCameraControllerType::kOrtho;
		m_camera_controller = ToyEngine::MakeScope<ToyEngine::CameraController>(camera_control_props);

		TY_INFO("Create lights...");
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
		// Update lights
		m_light_block->m_lights[0].m_direction		= glm::vec4(m_directional_light_dir,0.0f);
		m_light_block->m_lights[0].m_value			= glm::vec4(m_directional_light_color, 0.0f);
		m_light_block->m_lights[1].m_position		= glm::vec4(m_point_light_position_1, 0.0f);
		m_light_block->m_lights[1].m_value			= glm::vec4(m_point_light_color_1, 0.0f);
		m_light_block->m_lights[2].m_position		= glm::vec4(m_point_light_position_2, 0.0f);
		m_light_block->m_lights[2].m_value			= glm::vec4(m_point_light_color_2, 0.0f);
		m_light_block->m_lights[3].m_position		= glm::vec4(m_point_light_position_3, 0.0f);
		m_light_block->m_lights[3].m_value			= glm::vec4(m_point_light_color_3, 0.0f);

		if (m_viewport_hovered)
			m_camera_controller->Update(time_step);

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

		// Draw Scene to frame buffer
		m_frame_buffer->Bind();
		ToyEngine::Renderer::BeginScene(&m_camera_controller->GetCamera(), m_light_block.get());
		ToyEngine::Renderer::Submit(m_scene_graph.get());
		ToyEngine::Renderer::EndScene();
		m_frame_buffer->Unbind();

		//// Draw quad with post-processing 
		//ToyEngine::Ref<ToyEngine::Shader> postfxShader = m_shader_lib->Get("postfx");
		//postfxShader->Use();
		//postfxShader->SetInt("screenTexture", 0);
		//bool depth_test;
		//ToyEngine::RenderCommand::GetBooleanv(ToyEngine::eParamType::kDEPTH_TEST, &depth_test);
		//ToyEngine::RenderCommand::Disable(ToyEngine::eParamType::kDEPTH_TEST);
		//ToyEngine::RenderCommand::ClearSetBackground();
		//ToyEngine::RenderCommand::BindTexture(ToyEngine::eSamplerType::kTexture2D, m_frame_buffer->GetColorAttachment());
		//ToyEngine::Renderer::Submit(m_quad_vertex_array.get());
		//if (depth_test) ToyEngine::RenderCommand::Enable(ToyEngine::eParamType::kDEPTH_TEST);
	}

	virtual void OnImGuiRender() 
	{
		// Define the GUI windows
		// ------------------------------
		ImGuiIO& io_ = ImGui::GetIO(); (void)io_;

		// Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! 
		// You can browse its code to learn more about Dear ImGui!).
		ImGui::ShowDemoWindow();

		// Main Dockspace
		{
			static bool dockspace_open  = true;
			static bool opt_fullscreen_persistant = true; 
			bool opt_fullscreen = true;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
			// because it would be confusing to have two docking targets within each others.
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (opt_fullscreen) 
			{
				ImGuiViewport* viewport = ImGui::GetMainViewport(); 
				ImGui::SetNextWindowPos(viewport->Pos); 
				ImGui::SetNextWindowSize(viewport->Size); 
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}

			// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
			// and handle the pass-thru hole, so we ask Begin() to not render a background.
			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
			// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
			// all active windows docked into it will lose their parent and become undocked.
			// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
			// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f,0.0f)); 
			ImGui::Begin("DockSpace Demo", &dockspace_open, window_flags); 
			ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2); 

			// DockSpace
			if (io_.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace"); 
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Exit")) TY_WARN("TODO:: Implement application close option"); 
					ImGui::EndMenu(); 
				}
				ImGui::EndMenuBar(); 
			}
			
			// Viewport
			{
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
				ImGui::Begin("Viewport");

				m_viewport_focused = ImGui::IsWindowFocused();
				m_viewport_hovered = ImGui::IsWindowHovered();
				ToyEngine::Application::Get().GetImGuiLayer()->BlockEvents(!m_viewport_focused || !m_viewport_hovered);

				// Check if window was resized and recreate framebuffer if needed
				ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
				if (viewport_panel_size.x != m_frame_buffer->GetWidth() || viewport_panel_size.y != m_frame_buffer->GetHeight()) {
					m_frame_buffer->Resize((uint32_t)viewport_panel_size.x, (uint32_t)viewport_panel_size.y);
					m_camera_controller->OnResize(viewport_panel_size.x, viewport_panel_size.y);
				}

				ImGui::Image(m_frame_buffer->GetColorAttachment(),
					ImVec2(m_frame_buffer->GetWidth(), m_frame_buffer->GetHeight()),
					ImVec2(0, 1), ImVec2(1, 0));
				ImGui::End();
				ImGui::PopStyleVar();
			}

			// Scene Graph
			{
				ImGui::Begin("Scene Graph");
				DrawSceneNodeTree(m_scene_graph.get());
				ImGui::End();
			}

			// Controls
			{
				ImGui::Begin("Controls");
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io_.Framerate, io_.Framerate);

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
			}

			//
			ImGui::End();
		}
	}

	virtual void OnEvent(ToyEngine::Event& e)
	{
		if(m_viewport_hovered)
			m_camera_controller->OnEvent(e);
	}
public: 
	ToyEngine::Scope<ToyEngine::CameraController> m_camera_controller;
	ToyEngine::Scope<ToyEngine::ShaderLibrary> m_shader_lib;
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

	// viewport variables
	bool m_viewport_focused, m_viewport_hovered;
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
