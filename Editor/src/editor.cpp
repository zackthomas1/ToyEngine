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
		m_shader_lib->Load(ToyEngine::Shader::Create("flat_color", "../assets/shaders/flat_color.vs", "../assets/shaders/flat_color.fs"));
		m_shader_lib->Load(ToyEngine::Shader::Create("flat_texture", "../assets/shaders/flat_texture.vs", "../assets/shaders/flat_texture.fs"));
		m_shader_lib->Load(ToyEngine::Shader::Create("phong", "../assets/shaders/phong.vs", "../assets/shaders/phong.fs"));
		TY_INFO("Shader compilation complete");
		
        // Get the index of the "Matrices" uniform block in each shader program.
        // This index is used to refer to the block within the shader.
        uint32_t flat_color_block_index = glGetUniformBlockIndex(m_shader_lib->Get("flat_color")->id(), "Matrices");
        uint32_t flat_texture_block_index = glGetUniformBlockIndex(m_shader_lib->Get("flat_texture")->id(), "Matrices");
        uint32_t phong_block_index = glGetUniformBlockIndex(m_shader_lib->Get("phong")->id(), "Matrices");

        // Bind each uniform block index to a binding point (here, binding point 0).
        // This tells OpenGL that the "Matrices" block in each shader will use binding point 0.
        glUniformBlockBinding(m_shader_lib->Get("flat_color")->id(), flat_color_block_index, 0);
        glUniformBlockBinding(m_shader_lib->Get("flat_texture")->id(), flat_texture_block_index, 0);
        glUniformBlockBinding(m_shader_lib->Get("phong")->id(), phong_block_index, 0);

        // Create a uniform buffer object (UBO) to store the data for the uniform block.
        glGenBuffers(1, &ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo); 
        // Allocate space for two 4x4 matrices (view and projection) in the buffer.
        glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW); 
        glBindBuffer(GL_UNIFORM_BUFFER, 0); 
        // Bind the buffer to the same binding point (0) used above.
        // This links the buffer's data to the "Matrices" block in all shaders using binding point 0.
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, 2 * sizeof(glm::mat4));

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
        // Bind the uniform buffer object (UBO) to update its data.
        // The first glBufferSubData call uploads the camera's view matrix to the first half of the buffer.
        // The second glBufferSubData call uploads the camera's projection matrix to the second half of the buffer.
        // This ensures both matrices are available to all shaders using the "Matrices" uniform block.
        glBindBuffer(GL_UNIFORM_BUFFER, ubo); 
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(m_camera->GetViewMatrix()));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(m_camera->GetProjectionMatrix()));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
		// set model matrix and submit to render for drawing
		m_models[0]->m_model_mat = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation_degree), glm::vec3(0.0f, 1.0f, 0.0f));
		ToyEngine::Renderer::Submit(m_shader_lib->Get("flat_texture"), m_models[0]);
		m_models[1]->m_model_mat = glm::translate(glm::mat4(1.0f), m_translate);
		ToyEngine::Renderer::Submit(m_shader_lib->Get("flat_color"), m_models[1]);

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
	ToyEngine::Ref<ToyEngine::ShaderLibrary> m_shader_lib;

	//Models and model control variables
	ToyEngine::Vector<ToyEngine::Ref<ToyEngine::Model>> m_models;
	float m_rotation_degree = 0;
	glm::vec3 m_translate = glm::vec3(0.0f);
	//std::vector<Light> lights_;
	uint32_t ubo;


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
