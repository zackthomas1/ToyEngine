#include <toy_engine.h>
#include <glad/glad.h>

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
		ToyEngine::Ref<ToyEngine::Shader> flatShader = ToyEngine::Shader::Create("flat_color", "../assets/shaders/flat_color.vs", "../assets/shaders/flat_color.fs");
		ToyEngine::Ref<ToyEngine::Shader> textureShader = ToyEngine::Shader::Create("flat_texture", "../assets/shaders/flat_texture.vs", "../assets/shaders/flat_texture.fs");
		ToyEngine::Ref<ToyEngine::Shader> phongShader = ToyEngine::Shader::Create("phong", "../assets/shaders/phong.vs", "../assets/shaders/phong.fs");

		m_shader_lib->Add(flatShader);
		m_shader_lib->Add(textureShader);
		m_shader_lib->Add(phongShader);

		ToyEngine::Renderer::GetUniformManager().BindUniformBlockToShader(flatShader, "ViewProjectMats");
		ToyEngine::Renderer::GetUniformManager().BindUniformBlockToShader(textureShader, "ViewProjectMats");
		ToyEngine::Renderer::GetUniformManager().BindUniformBlockToShader(phongShader, "ViewProjectMats");

		ToyEngine::Renderer::GetUniformManager().BindUniformBlockToShader(flatShader, "LightBlock");
		ToyEngine::Renderer::GetUniformManager().BindUniformBlockToShader(textureShader, "LightBlock");
		ToyEngine::Renderer::GetUniformManager().BindUniformBlockToShader(phongShader, "LightBlock");

	}

	virtual void OnAttach()
	{
		TY_INFO("Initializing Camera...");
		m_camera = ToyEngine::MakeRef<ToyEngine::Camera>(ToyEngine::eCameraType::kFlyCamera);

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

		// Create scene geometry
		TY_INFO("Create scene...");
		m_scene_graph = ToyEngine::MakeScope<ToyEngine::SceneNode>("root");
		ToyEngine::Ref<ToyEngine::Model> backpack	= (ToyEngine::Model::Create("../assets/models/backpack/backpack.obj", true));
		ToyEngine::Ref<ToyEngine::Model> cyborg		= (ToyEngine::Model::Create("../assets/models/cyborg/cyborg.obj", false));
		ToyEngine::Ref<ToyEngine::Shader> phongShader = m_shader_lib->Get("phong");
		backpack->m_shader = phongShader;
		cyborg->m_shader = phongShader;
		m_scene_graph->AddChild(ToyEngine::MakeScope<ToyEngine::SceneNode>("backepack_model", backpack));
		m_scene_graph->AddChild(ToyEngine::MakeScope<ToyEngine::SceneNode>("cyborg_model", cyborg));
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

		// Draw Scene
		ToyEngine::Renderer::BeginScene(m_camera, m_light_block.get());
		ToyEngine::Ref<ToyEngine::Shader> phongShader = m_shader_lib->Get("phong");
		// models
		phongShader->SetFloat("material.shininess", m_shininess);
		m_scene_graph->SetLocalTransform(glm::translate(glm::mat4(1.0f), m_translate));
		m_scene_graph->GetChildren()[1]->SetLocalTransform(
			glm::rotate(glm::translate(glm::mat4(1.0f), m_translate_cyborg),
				glm::radians(m_rotation_degree), glm::vec3(0.0f, 1.0f, 0.0f)));
		ToyEngine::Renderer::Submit(m_scene_graph.get());
		ToyEngine::Renderer::EndScene();
	}

	virtual void OnImGuiRender() 
	{
		// Define the GUI windows
		// ------------------------------
		ImGuiIO& io_ = ImGui::GetIO(); (void)io_;

		// Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! 
		// You can browse its code to learn more about Dear ImGui!).
		ImGui::ShowDemoWindow();

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
			ImGui::DragFloat("Material Shininess##MaterialShininess", &m_shininess, 0.1f, 0.1f, 256.0f, "%.1f");
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
	ToyEngine::Ref<ToyEngine::Camera> m_camera;

	// Model control parameters
	ToyEngine::Scope<ToyEngine::SceneNode> m_scene_graph;
	float m_rotation_degree = 0;
	glm::vec3 m_translate = glm::vec3(0.0f), m_translate_cyborg = glm::vec3(0.0f);
	float m_shininess = 32.0f;

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
