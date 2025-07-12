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
		ToyEngine::Ref<ToyEngine::Shader> advancedLightingShader = ToyEngine::Shader::Create("advanced_lighting", "../assets/shaders/advanced_lighting.vs", "../assets/shaders/advanced_lighting.fs");

		m_shader_lib->Add(flatShader);
		m_shader_lib->Add(textureShader);
		m_shader_lib->Add(phongShader);
		m_shader_lib->Add(advancedLightingShader);

		uint32_t matrices_bind_point = ToyEngine::Renderer::GetUniformManager().GetBindPoint("ViewProjectMats");
		flatShader->BindUniformBlock("ViewProjectMats", matrices_bind_point);
		textureShader->BindUniformBlock("ViewProjectMats", matrices_bind_point);
		phongShader->BindUniformBlock("ViewProjectMats", matrices_bind_point);
		advancedLightingShader->BindUniformBlock("ViewProjectMats", matrices_bind_point);
		TY_INFO("Shader compilation complete");

		// Setup lighting
		SetupLighting();
	}

	void SetupLighting()
	{
		ToyEngine::LightManager& lightManager = ToyEngine::Renderer::GetLightManager();
		
		// Clear existing lights and add our custom lights
		lightManager.ClearAll();
		
		// Add a main directional light (sun)
		ToyEngine::DirectionalLight sunLight;
		sunLight.direction = glm::vec3(0.5f, -1.0f, 0.3f);
		sunLight.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
		sunLight.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		sunLight.specular = glm::vec3(0.5f, 0.5f, 0.5f);
		lightManager.AddDirectionalLight(sunLight);
		
		// Add a point light
		ToyEngine::PointLight pointLight;
		pointLight.position = glm::vec3(2.0f, 2.0f, 2.0f);
		pointLight.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
		pointLight.diffuse = glm::vec3(0.8f, 0.2f, 0.2f); // Red tint
		pointLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
		pointLight.constant = 1.0f;
		pointLight.linear = 0.09f;
		pointLight.quadratic = 0.032f;
		lightManager.AddPointLight(pointLight);
		
		// Add a spot light
		ToyEngine::SpotLight spotLight;
		spotLight.position = glm::vec3(0.0f, 4.0f, 0.0f);
		spotLight.direction = glm::vec3(0.0f, -1.0f, 0.0f);
		spotLight.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
		spotLight.diffuse = glm::vec3(0.0f, 0.0f, 1.0f); // Blue tint
		spotLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
		spotLight.innerCone = glm::cos(glm::radians(12.5f));
		spotLight.outerCone = glm::cos(glm::radians(17.5f));
		spotLight.constant = 1.0f;
		spotLight.linear = 0.09f;
		spotLight.quadratic = 0.032f;
		lightManager.AddSpotLight(spotLight);
		
		TY_INFO("Lighting setup complete - {} directional, {} point, {} spot lights", 
			lightManager.GetDirectionalLightCount(), 
			lightManager.GetPointLightCount(), 
			lightManager.GetSpotLightCount());
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

		// Update dynamic lights
		UpdateLights();

		// Draw Scene
		ToyEngine::Renderer::BeginScene(m_camera);
		
		// Use the advanced lighting shader
		ToyEngine::Ref<ToyEngine::Shader> lightingShader = m_shader_lib->Get(m_useAdvancedLighting ? "advanced_lighting" : "phong");
		lightingShader->Use();
		
		// Set material properties
		lightingShader->SetFloat("material.shininess", m_shininess);
		
		// Update lighting uniforms
		if (m_useAdvancedLighting) {
			ToyEngine::Renderer::GetLightManager().UpdateShaderUniforms(lightingShader, m_camera->GetPosition());
		} else {
			// Legacy single directional light for phong shader
			lightingShader->SetFloat3("light.direction", m_light_direction.x, m_light_direction.y, m_light_direction.z);
			lightingShader->SetFloat3("light.ambient", 0.2f, 0.2f, 0.2f);
			lightingShader->SetFloat3("light.diffuse", 1.0f, 1.0f, 1.0f);
			lightingShader->SetFloat3("light.specular", 0.5f, 0.5f, 0.5f);
			lightingShader->SetFloat3("viewPos", m_camera->GetPosition().x, m_camera->GetPosition().y, m_camera->GetPosition().z);
		}
		
		// Render models
		m_models[0]->m_model_mat = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation_degree), glm::vec3(0.0f, 1.0f, 0.0f));
		ToyEngine::Renderer::Submit(lightingShader, m_models[0]);
		m_models[1]->m_model_mat = glm::translate(glm::mat4(1.0f), m_translate);
		ToyEngine::Renderer::Submit(lightingShader, m_models[1]);
		ToyEngine::Renderer::EndScene();
	}

	void UpdateLights()
	{
		ToyEngine::LightManager& lightManager = ToyEngine::Renderer::GetLightManager();
		
		// Update point light position to orbit around the scene
		static float time = 0.0f;
		time += 0.016f; // Assume ~60 FPS
		
		ToyEngine::PointLight* pointLight = lightManager.GetPointLight(0);
		if (pointLight) {
			pointLight->position = glm::vec3(
				sin(time) * 3.0f,
				2.0f,
				cos(time) * 3.0f
			);
		}
		
		// Update spot light to follow the camera
		ToyEngine::SpotLight* spotLight = lightManager.GetSpotLight(0);
		if (spotLight && m_enableSpotLightFollowCamera) {
			spotLight->position = m_camera->GetPosition();
			spotLight->direction = m_camera->GetFront();
		}
	}

	virtual void OnImGuiRender() 
	{
		// Define the GUI windows
		// ------------------------------
		ImGuiIO& io_ = ImGui::GetIO(); (void)io_;

		// Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! 
		// You can browse its code to learn more about Dear ImGui!).
		if (m_showDemoWindow) {
			ImGui::ShowDemoWindow(&m_showDemoWindow);
		}

		// Show lighting control window
		ImGui::Begin("Lighting System");
		
		// General settings
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io_.Framerate, io_.Framerate);
		ImGui::Text("Camera FOV: %d", static_cast<int>(m_camera->fov()));
		ImGui::Separator();
		
		// Scene controls
		ImGui::Text("Scene Controls");
		ImGui::Checkbox("Use Advanced Lighting", &m_useAdvancedLighting);
		ImGui::Checkbox("Show Demo Window", &m_showDemoWindow);
		ImGui::DragFloat("Rotate Backpack", &m_rotation_degree, 0.1f, 0.0f, 360.0f, "%.1f", ImGuiSliderFlags_WrapAround);
		ImGui::DragFloat("Material Shininess", &m_shininess, 0.1f, 0.1f, 256.0f, "%.1f");
		ImGui::DragFloat3("Translate Cyborg", glm::value_ptr(m_translate), 0.1f, -10.0f, 10.0f, "%.1f");
		ImGui::Separator();
		
		// Lighting controls
		ToyEngine::LightManager& lightManager = ToyEngine::Renderer::GetLightManager();
		
		ImGui::Text("Lights: %d Directional, %d Point, %d Spot", 
			lightManager.GetDirectionalLightCount(), 
			lightManager.GetPointLightCount(), 
			lightManager.GetSpotLightCount());
		
		// Legacy directional light control (for phong shader)
		if (!m_useAdvancedLighting) {
			ImGui::Text("Legacy Directional Light");
			ImGui::DragFloat3("Light Direction", glm::value_ptr(m_light_direction), 0.01f, -1.0f, 1.0f, "%.2f");
		}
		
		// Advanced lighting controls
		if (m_useAdvancedLighting) {
			ImGui::Text("Advanced Lighting Controls");
			
			// Directional light controls
			if (ImGui::CollapsingHeader("Directional Lights")) {
				for (int i = 0; i < lightManager.GetDirectionalLightCount(); ++i) {
					ToyEngine::DirectionalLight* light = lightManager.GetDirectionalLight(i);
					if (light) {
						ImGui::PushID(i);
						ImGui::Text("Directional Light %d", i);
						ImGui::Checkbox("Enabled", &light->enabled);
						ImGui::DragFloat3("Direction", glm::value_ptr(light->direction), 0.01f, -1.0f, 1.0f, "%.2f");
						ImGui::ColorEdit3("Ambient", glm::value_ptr(light->ambient));
						ImGui::ColorEdit3("Diffuse", glm::value_ptr(light->diffuse));
						ImGui::ColorEdit3("Specular", glm::value_ptr(light->specular));
						ImGui::Separator();
						ImGui::PopID();
					}
				}
			}
			
			// Point light controls
			if (ImGui::CollapsingHeader("Point Lights")) {
				for (int i = 0; i < lightManager.GetPointLightCount(); ++i) {
					ToyEngine::PointLight* light = lightManager.GetPointLight(i);
					if (light) {
						ImGui::PushID(i + 100);
						ImGui::Text("Point Light %d", i);
						ImGui::Checkbox("Enabled", &light->enabled);
						ImGui::DragFloat3("Position", glm::value_ptr(light->position), 0.1f, -10.0f, 10.0f, "%.1f");
						ImGui::ColorEdit3("Ambient", glm::value_ptr(light->ambient));
						ImGui::ColorEdit3("Diffuse", glm::value_ptr(light->diffuse));
						ImGui::ColorEdit3("Specular", glm::value_ptr(light->specular));
						ImGui::DragFloat("Constant", &light->constant, 0.01f, 0.0f, 2.0f, "%.2f");
						ImGui::DragFloat("Linear", &light->linear, 0.001f, 0.0f, 1.0f, "%.3f");
						ImGui::DragFloat("Quadratic", &light->quadratic, 0.001f, 0.0f, 1.0f, "%.3f");
						ImGui::Separator();
						ImGui::PopID();
					}
				}
			}
			
			// Spot light controls
			if (ImGui::CollapsingHeader("Spot Lights")) {
				for (int i = 0; i < lightManager.GetSpotLightCount(); ++i) {
					ToyEngine::SpotLight* light = lightManager.GetSpotLight(i);
					if (light) {
						ImGui::PushID(i + 200);
						ImGui::Text("Spot Light %d", i);
						ImGui::Checkbox("Enabled", &light->enabled);
						ImGui::Checkbox("Follow Camera", &m_enableSpotLightFollowCamera);
						if (!m_enableSpotLightFollowCamera) {
							ImGui::DragFloat3("Position", glm::value_ptr(light->position), 0.1f, -10.0f, 10.0f, "%.1f");
							ImGui::DragFloat3("Direction", glm::value_ptr(light->direction), 0.01f, -1.0f, 1.0f, "%.2f");
						}
						ImGui::ColorEdit3("Ambient", glm::value_ptr(light->ambient));
						ImGui::ColorEdit3("Diffuse", glm::value_ptr(light->diffuse));
						ImGui::ColorEdit3("Specular", glm::value_ptr(light->specular));
						
						// Convert cone angles to degrees for UI
						float innerDegrees = glm::degrees(glm::acos(light->innerCone));
						float outerDegrees = glm::degrees(glm::acos(light->outerCone));
						if (ImGui::DragFloat("Inner Cone (degrees)", &innerDegrees, 0.1f, 0.0f, 90.0f, "%.1f")) {
							light->innerCone = glm::cos(glm::radians(innerDegrees));
						}
						if (ImGui::DragFloat("Outer Cone (degrees)", &outerDegrees, 0.1f, 0.0f, 90.0f, "%.1f")) {
							light->outerCone = glm::cos(glm::radians(outerDegrees));
						}
						
						ImGui::DragFloat("Constant", &light->constant, 0.01f, 0.0f, 2.0f, "%.2f");
						ImGui::DragFloat("Linear", &light->linear, 0.001f, 0.0f, 1.0f, "%.3f");
						ImGui::DragFloat("Quadratic", &light->quadratic, 0.001f, 0.0f, 1.0f, "%.3f");
						ImGui::Separator();
						ImGui::PopID();
					}
				}
			}
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
	ToyEngine::Ref<ToyEngine::Camera> m_camera;
	ToyEngine::Ref<ToyEngine::UniformBuffer> m_matrix_uniform_buffer;
	ToyEngine::Ref<ToyEngine::ShaderLibrary> m_shader_lib;

	// Models and model control variables
	ToyEngine::Vector<ToyEngine::Ref<ToyEngine::Model>> m_models;
	float m_rotation_degree = 0;
	float m_shininess = 32.0f;
	glm::vec3 m_translate = glm::vec3(0.0f);
	glm::vec3 m_light_direction = glm::vec3(1.0f, 0.0f, 0.0f);

	// Lighting system variables
	bool m_useAdvancedLighting = true;
	bool m_showDemoWindow = false;
	bool m_enableSpotLightFollowCamera = false;
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
