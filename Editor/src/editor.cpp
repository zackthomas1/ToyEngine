#include <toy_engine.h>
#include <glad/glad.h>

class Scene : public ToyEngine::Layer
{
public:
	Scene()
	{
		TY_INFO("Initialize Camera");
		m_camera = ToyEngine::MakeRef<ToyEngine::Camera>(ToyEngine::eCameraType::kFlyCamera);
		m_shader_lib = ToyEngine::MakeRef<ToyEngine::ShaderLibrary>();

		TY_INFO("Compiling shaders...");
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
		TY_INFO("Shader compilation complete");
	}

	virtual void OnAttach() 
	{
		// Create scene geometry
		TY_INFO("Loading scene geometry...");
		m_models.push_back(ToyEngine::Model::Create("../assets/models/backpack/backpack.obj", true));
		m_models.push_back(ToyEngine::Model::Create("../assets/models/cyborg/cyborg.obj", false));
		TY_INFO("Scene loading complete");
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

		// Draw Scene
		ToyEngine::Renderer::BeginScene(m_camera);
		
		// set model matrix and submit to render for drawing
		m_models[0]->m_model_mat = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation_degree), glm::vec3(0.0f, 1.0f, 0.0f));
		ToyEngine::Renderer::Submit(m_shader_lib->Get("flat_texture"), m_models[0]);
		m_models[1]->m_model_mat = glm::translate(glm::mat4(1.0f), m_translate);
		ToyEngine::Ref<ToyEngine::Shader> phongShader = m_shader_lib->Get("phong");
		phongShader->Use();
		phongShader->SetFloat3("light.direction", 1.0f, 0.5f, 0.0f);
		phongShader->SetFloat3("light.ambient", 0.2f, 0.2f, 0.2f);
		phongShader->SetFloat3("light.diffuse", 1.0f, 1.0f, 1.0f);
		phongShader->SetFloat3("light.specular", 0.5f, 0.5f, 0.5f);
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
        ImGui::Text("Camera FOV: %d", static_cast<int>(m_camera->fov()));
		ImGui::DragFloat("Rotate", &m_rotation_degree, 0.1f, 0.0f, 360.0f, "%0.1f", ImGuiSliderFlags_WrapAround);
		ImGui::InputFloat3("Translate", glm::value_ptr(m_translate), "%0.1f");
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
	ToyEngine::Ref<ToyEngine::Camera> m_camera;
	ToyEngine::Ref<ToyEngine::UniformBuffer> m_matrix_uniform_buffer;
	ToyEngine::Ref<ToyEngine::ShaderLibrary> m_shader_lib;

	//Models and model control variables
	ToyEngine::Vector<ToyEngine::Ref<ToyEngine::Model>> m_models;
	float m_rotation_degree = 0;
	glm::vec3 m_translate = glm::vec3(0.0f);

	//std::vector<Light> lights_;

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
