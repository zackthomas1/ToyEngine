#include <toy_engine.h>
#include <glad/glad.h>

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

		uint32_t matrices_bind_point = ToyEngine::Renderer::GetUniformManager().GetBindPoint("ViewProjectMats");
		flatShader->BindUniformBlock("ViewProjectMats", matrices_bind_point);
		textureShader->BindUniformBlock("ViewProjectMats", matrices_bind_point);
		phongShader->BindUniformBlock("ViewProjectMats", matrices_bind_point);
	}

	virtual void OnAttach()
	{
		TY_INFO("Initializing Camera...");
		m_camera = ToyEngine::MakeRef<ToyEngine::Camera>(ToyEngine::eCameraType::kFlyCamera);

		TY_INFO("Creating lights...");
		m_lights.push_back(ToyEngine::MakeRef<ToyEngine::Light>(ToyEngine::eLightType::kDirectional));
		m_lights.push_back(ToyEngine::MakeRef<ToyEngine::Light>(ToyEngine::eLightType::kPoint));
		m_lights.push_back(ToyEngine::MakeRef<ToyEngine::Light>(ToyEngine::eLightType::kPoint));
		m_lights.push_back(ToyEngine::MakeRef<ToyEngine::Light>(ToyEngine::eLightType::kPoint));
		m_lights.push_back(ToyEngine::MakeRef<ToyEngine::Light>(ToyEngine::eLightType::kSpot));

		// Create scene geometry
		TY_INFO("Loading scene geometry...");
		m_models.push_back(ToyEngine::Model::Create("../assets/models/backpack/backpack.obj", true));
		m_models.push_back(ToyEngine::Model::Create("../assets/models/cyborg/cyborg.obj", false));
		TY_INFO("Scene loaded!");
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
		m_lights[0]->m_direction = m_directional_light_dir;
		m_lights[0]->m_value = m_directional_light_color;
		m_lights[1]->m_position = m_point_light_position_1;
		m_lights[1]->m_value = m_point_light_color_1;
		m_lights[2]->m_position = m_point_light_position_2;
		m_lights[2]->m_value = m_point_light_color_2;
		m_lights[3]->m_position = m_point_light_position_3;
		m_lights[3]->m_value = m_point_light_color_3;
		m_lights[4]->m_position = m_camera->position();
		m_lights[4]->m_spotDirection = m_camera->front();
		m_lights[4]->m_value = m_spot_light_color;

		//TY_CORE_TRACE("({},{},{})", m_camera->front().x, m_camera->front().y, m_camera->front().z);

		// Draw Scene
		ToyEngine::Renderer::BeginScene(m_camera);
		// set model matrix and submit to render for drawing
		ToyEngine::Ref<ToyEngine::Shader> phongShader = m_shader_lib->Get("phong");
		phongShader->Use();
		// Lights
		int numDirection = 0, numPoint = 0, numSpot = 0;
		for (int i = 0; i < m_lights.size(); i++) {
			ToyEngine::Ref<ToyEngine::Light> light = m_lights[i];
			switch (light->m_type)
			{
			case(ToyEngine::eLightType::kDirectional): {
				std::string base = "uDirectionalLights[" + std::to_string(numDirection++) + "]";
				phongShader->SetFloat3(base + ".direction", light->m_direction.x, light->m_direction.y, light->m_direction.z);
				phongShader->SetFloat3(base + ".value", light->m_value.r, light->m_value.g, light->m_value.b);
				phongShader->SetBool(base + ".enabled", light->m_enabled);
				phongShader->SetInt("uNumDirectionalLights", (numDirection));
				break;
			}
			case(ToyEngine::eLightType::kPoint):{
				std::string base = "uPointLights[" + std::to_string(numPoint++) + "]";
				phongShader->SetFloat3(base + ".position", light->m_position.x, light->m_position.y, light->m_position.z);
				phongShader->SetFloat3(base + ".value", light->m_value.r, light->m_value.g, light->m_value.b);
				phongShader->SetBool(base + ".enabled", light->m_enabled);
				phongShader->SetInt("uNumPointLights", (numPoint));
				break; 
			}
			case(ToyEngine::eLightType::kSpot):{
				std::string base = "uSpotLights[" + std::to_string(numSpot++) + "]";
				phongShader->SetFloat3(base + ".position", light->m_position.x, light->m_position.y, light->m_position.z);
				phongShader->SetFloat3(base + ".value", light->m_value.r, light->m_value.g, light->m_value.b);
				phongShader->SetFloat3(base + ".spotDirection", light->m_spotDirection.x, light->m_spotDirection.y, light->m_spotDirection.z);
				phongShader->SetFloat(base + ".innerAngle", light->m_innerAngle);
				phongShader->SetFloat(base + ".outerAngle", light->m_outerAngle);
				phongShader->SetBool(base + ".enabled", light->m_enabled);
				phongShader->SetInt("uNumSpotLights", (numSpot));
				break;
			}
			default:
				break;
			}
		}
		phongShader->SetFloat("material.shininess", m_shininess);
		m_models[0]->m_model_mat = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation_degree), glm::vec3(0.0f, 1.0f, 0.0f));
		ToyEngine::Renderer::Submit(phongShader, m_models[0]);
		m_models[1]->m_model_mat = glm::translate(glm::mat4(1.0f), m_translate);
		ToyEngine::Renderer::Submit(phongShader, m_models[1]);
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
		ImGui::Begin("Hello, World");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io_.Framerate, io_.Framerate);
		ImGui::Text("Scene Controls");
		ImGui::Text("Camera FOV: %d", static_cast<int>(m_camera->fov()));
		ImGui::DragFloat("Rotate Backpack##RotateBackpack", &m_rotation_degree, 0.1f, 0.0f, 360.0f, "%.1f", ImGuiSliderFlags_WrapAround);
		ImGui::DragFloat3("Translate Cyborg##TranslateCyborg", glm::value_ptr(m_translate), 0.1f, -10.0f, 10.0f, "%.1f");
		ImGui::DragFloat("Material Shininess##MaterialShininess", &m_shininess, 0.1f, 0.1f, 256.0f, "%.1f");

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
	ToyEngine::Vector<ToyEngine::Ref<ToyEngine::Model>> m_models;
	float m_rotation_degree = 0;
	glm::vec3 m_translate = glm::vec3(0.0f);
	float m_shininess = 32.0f;

	// Light control parameters
	ToyEngine::Vector<ToyEngine::Ref<ToyEngine::Light>> m_lights;
	glm::vec3 m_directional_light_color = glm::vec3(0.2), m_directional_light_dir = glm::vec3(0.0, 0.0, -1.0);
	glm::vec3 m_point_light_color_1 = glm::vec3(1.0, 0.0, 0.0), m_point_light_color_2 = glm::vec3(0.0, 1.0, 0.0), m_point_light_color_3 = glm::vec3(0.0, 0.0, 1.0);
	glm::vec3 m_point_light_position_1 = glm::vec3(1.0, 0.0, 0.0), m_point_light_position_2 = glm::vec3(0.0, 1.0, 0.0), m_point_light_position_3 = glm::vec3(0.0, 0.0, 1.0);
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
	~Editor()
	{

	}
};

ToyEngine::Application* ToyEngine::CreateApplication()
{
	return new Editor();
}
