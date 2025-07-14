#include <toy_engine.h>
#include <glad/glad.h>

// Example Scene Layer that demonstrates the ECS system
class ECSScene : public ToyEngine::Layer
{
public:
	ECSScene()
	{
		TY_INFO("Initialize ECS Scene");
		
		// Initialize camera
		m_camera = ToyEngine::MakeRef<ToyEngine::Camera>(ToyEngine::eCameraType::kFlyCamera);
		m_shader_lib = ToyEngine::MakeRef<ToyEngine::ShaderLibrary>();

		// Create scene
		m_scene = ToyEngine::MakeScope<ToyEngine::Scene>("Main Scene");

		TY_INFO("Compiling shaders...");
		ToyEngine::Ref<ToyEngine::Shader> flatShader = ToyEngine::Shader::Create("flat_color", "../assets/shaders/flat_color.vs", "../assets/shaders/flat_color.fs");
		ToyEngine::Ref<ToyEngine::Shader> textureShader = ToyEngine::Shader::Create("flat_texture", "../assets/shaders/flat_texture.vs", "../assets/shaders/flat_texture.fs");
		ToyEngine::Ref<ToyEngine::Shader> phongShader = ToyEngine::Shader::Create("phong", "../assets/shaders/phong.vs", "../assets/shaders/phong.fs");

		m_shader_lib->Add(flatShader);
		m_shader_lib->Add(textureShader);
		m_shader_lib->Add(phongShader);

		uint32_t matrices_bind_point = ToyEngine::Renderer::GetUniformManager().GetBindPoint("Matrices");
		flatShader->BindUniformBlock("Matrices", matrices_bind_point);
		textureShader->BindUniformBlock("Matrices", matrices_bind_point);
		phongShader->BindUniformBlock("Matrices", matrices_bind_point);
		TY_INFO("Shader compilation complete");
	}

	virtual void OnAttach() 
	{
		TY_INFO("Loading scene geometry using ECS...");
		
		// Create first entity with backpack model
		ToyEngine::Entity& backpack_entity = m_scene->CreateEntity("Backpack");
		ToyEngine::Transform& backpack_transform = backpack_entity.GetComponent<ToyEngine::Transform>();
		backpack_transform.SetLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		
		// Add renderable component
		ToyEngine::Renderable& backpack_renderable = backpack_entity.AddComponent<ToyEngine::Renderable>();
		backpack_renderable.SetModel(ToyEngine::Model::Create("../assets/models/backpack/backpack.obj", true));
		backpack_renderable.SetShader(m_shader_lib->Get("flat_texture"));
		backpack_renderable.SetRenderLayer(0);

		// Create second entity with cyborg model
		ToyEngine::Entity& cyborg_entity = m_scene->CreateEntity("Cyborg");
		ToyEngine::Transform& cyborg_transform = cyborg_entity.GetComponent<ToyEngine::Transform>();
		cyborg_transform.SetLocalPosition(glm::vec3(3.0f, 0.0f, 0.0f));
		
		// Add renderable component
		ToyEngine::Renderable& cyborg_renderable = cyborg_entity.AddComponent<ToyEngine::Renderable>();
		cyborg_renderable.SetModel(ToyEngine::Model::Create("../assets/models/cyborg/cyborg.obj", false));
		cyborg_renderable.SetShader(m_shader_lib->Get("flat_color"));
		cyborg_renderable.SetRenderLayer(0);

		// Create a parent entity for hierarchical transforms
		ToyEngine::Entity& parent_entity = m_scene->CreateEntity("Parent");
		ToyEngine::Transform& parent_transform = parent_entity.GetComponent<ToyEngine::Transform>();
		parent_transform.SetLocalPosition(glm::vec3(0.0f, 2.0f, 0.0f));

		// Create child entity
		ToyEngine::Entity& child_entity = m_scene->CreateEntity("Child");
		ToyEngine::Transform& child_transform = child_entity.GetComponent<ToyEngine::Transform>();
		child_transform.SetLocalPosition(glm::vec3(1.0f, 0.0f, 0.0f));
		
		// Set up parent-child relationship
		m_scene->SetEntityParent(parent_entity, child_entity);
		
		// Store references for manipulation
		m_backpack_entity = &backpack_entity;
		m_cyborg_entity = &cyborg_entity;
		m_parent_entity = &parent_entity;
		m_child_entity = &child_entity;
		
		TY_INFO("ECS Scene loading complete");
		TY_INFO("Created {} entities in scene", m_scene->GetEntityCount());
	}

	virtual void OnDetach() {}

	virtual void Update(const ToyEngine::TimeStep& time_step)
	{
		// Handle input
		float delta_time = time_step.GetTimeDelta();
		ToyEngine::InputPoll& input = ToyEngine::Locator::InputPollService();
		
		// Camera movement
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

		// Update entity transforms based on UI controls
		if (m_backpack_entity != nullptr)
		{
			ToyEngine::Transform& transform = m_backpack_entity->GetComponent<ToyEngine::Transform>();
			transform.SetLocalRotation(glm::angleAxis(glm::radians(m_rotation_degree), glm::vec3(0.0f, 1.0f, 0.0f)));
		}
		
		if (m_cyborg_entity != nullptr)
		{
			ToyEngine::Transform& transform = m_cyborg_entity->GetComponent<ToyEngine::Transform>();
			transform.SetLocalPosition(m_translate);
		}

		// Update the scene (this will update all entities and their components)
		m_scene->Update(delta_time);

		// Render the scene using the ECS renderer
		ToyEngine::SceneRenderer::RenderScene(*m_scene, m_camera);
	}

	virtual void OnImGuiRender() 
	{
		// Define the GUI windows
		// ------------------------------
		ImGuiIO& io_ = ImGui::GetIO(); (void)io_;

		// Show the big demo window
		ImGui::ShowDemoWindow();

		// Show ECS info window
		ImGui::Begin("ECS Scene Info");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io_.Framerate, io_.Framerate);
		ImGui::Text("Camera FOV: %d", static_cast<int>(m_camera->fov()));
		ImGui::Text("Scene: %s", m_scene->GetName().c_str());
		ImGui::Text("Entity Count: %zu", m_scene->GetEntityCount());
		
		// Get entity statistics
		auto transform_entities = m_scene->GetEntitiesWithComponent<ToyEngine::Transform>();
		auto renderable_entities = m_scene->GetEntitiesWithComponent<ToyEngine::Renderable>();
		auto root_entities = m_scene->GetRootEntities();
		
		ImGui::Text("Entities with Transform: %zu", transform_entities.size());
		ImGui::Text("Entities with Renderable: %zu", renderable_entities.size());
		ImGui::Text("Root Entities: %zu", root_entities.size());
		
		ImGui::Separator();
		
		// Entity controls
		ImGui::Text("Entity Controls:");
		ImGui::DragFloat("Backpack Rotation", &m_rotation_degree, 0.1f, 0.0f, 360.0f, "%0.1f", ImGuiSliderFlags_WrapAround);
		ImGui::InputFloat3("Cyborg Position", glm::value_ptr(m_translate), "%0.1f");
		
		if (ImGui::Button("Reset Transforms"))
		{
			m_rotation_degree = 0.0f;
			m_translate = glm::vec3(3.0f, 0.0f, 0.0f);
		}
		
		// Entity visibility controls
		if (m_backpack_entity != nullptr && m_backpack_entity->HasComponent<ToyEngine::Renderable>())
		{
			ToyEngine::Renderable& renderable = m_backpack_entity->GetComponent<ToyEngine::Renderable>();
			bool visible = renderable.IsVisible();
			if (ImGui::Checkbox("Backpack Visible", &visible))
			{
				renderable.SetVisible(visible);
			}
		}
		
		if (m_cyborg_entity != nullptr && m_cyborg_entity->HasComponent<ToyEngine::Renderable>())
		{
			ToyEngine::Renderable& renderable = m_cyborg_entity->GetComponent<ToyEngine::Renderable>();
			bool visible = renderable.IsVisible();
			if (ImGui::Checkbox("Cyborg Visible", &visible))
			{
				renderable.SetVisible(visible);
			}
		}
		
		ImGui::End();
	}

	virtual void OnEvent(ToyEngine::Event& e)
	{
		// Set flag indicating that event has been handled by current layer
		e.SetEventHandled(true);
		
		if (ToyEngine::EventVerticalScroll* event = dynamic_cast<ToyEngine::EventVerticalScroll*>(&e)) {
			m_camera->UpdateFOV(event->GetYOffset());
		}

		if (ToyEngine::EventCursorPos* event = dynamic_cast<ToyEngine::EventCursorPos*>(&e))
		{
			m_camera->UpdateLookDirection(static_cast<float>(event->GetXOffset()), static_cast<float>(event->GetYOffset()));
		}
	}

private:
	ToyEngine::Ref<ToyEngine::Camera> m_camera;
	ToyEngine::Ref<ToyEngine::ShaderLibrary> m_shader_lib;
	ToyEngine::Scope<ToyEngine::Scene> m_scene;

	// Entity references for manipulation
	ToyEngine::Entity* m_backpack_entity = nullptr;
	ToyEngine::Entity* m_cyborg_entity = nullptr;
	ToyEngine::Entity* m_parent_entity = nullptr;
	ToyEngine::Entity* m_child_entity = nullptr;

	// Control variables
	float m_rotation_degree = 0.0f;
	glm::vec3 m_translate = glm::vec3(3.0f, 0.0f, 0.0f);
};

class Editor : public ToyEngine::Application
{
public:
	Editor()
	{
		TY_INFO("Initialize application with ECS");
		PushLayer(new ECSScene());
	}
	~Editor()
	{

	}
};

ToyEngine::Application* ToyEngine::CreateApplication()
{
	return new Editor();
}