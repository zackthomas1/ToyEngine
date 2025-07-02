#include "pch.h"
#include "scene.h"

#include "ToyEngine/renderer/mesh/primatives/cube_mesh.h"
#include "ToyEngine/renderer/mesh/primatives/triangle_mesh.h"
#include "ToyEngine/renderer/mesh/primatives/plane_mesh.h"

#include "ToyEngine/renderer/camera/fly_camera.h"
#include "ToyEngine/renderer/camera/orthographic_camera.h"

#include "ToyEngine/services/locator.h"

namespace ToyEngine
{
	// TODO: Remove cube position array.
	// This is for testing
	glm::vec3 cube_positions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
	};

	Scene::Scene()
	{
		// Create scene cameras
		camera_ = new FlyCamera();

		// Create scene geometry
		std::shared_ptr<CubeMesh> mesh = std::make_shared<CubeMesh>();
		std::shared_ptr<Material> material = std::make_shared<Material>();

		for (int i = 0; i < sizeof(cube_positions) / sizeof(glm::vec3); i++)
		{
			std::shared_ptr<Model> cube = std::make_shared<Model>(mesh, material);

			glm::mat4 transforms = glm::mat4(1.0f);
			transforms = glm::translate(transforms, cube_positions[i]);
			transforms = glm::rotate(transforms, 20.0f * i, glm::vec3(1.0f, 0.3f, 0.5f));
			cube->SetModelMatrix(transforms);

			models_.push_back(cube);
		}
	}

	Scene::~Scene()
	{
		delete camera_;
	}

	void Scene::Update(TimeStep *time_step)
	{
		//Cube boxes
		for(int i = 0; i < GetModels().size(); i++)
		{
			std::shared_ptr<Model> model = GetModels()[i];
			glm::mat4 transforms = model->GetModelMatrix();
			
			if (i % 2 == 0)
				transforms = glm::rotate(transforms, glm::radians(180.0f * time_step->GetTimeStep()), glm::vec3(1.0f, 0.3f * i, 0.5f * i));
			
			model->SetModelMatrix(transforms);
		}

		//Keyboard input polling
		float step_value = Locator::TimeStepService()->GetTimeStep();
		InputPoll *input = Locator::InputPollService();
		if (input->Key(KeyCode::kKeyW) != KeyState::kRelease)
		{
			camera_->UpdatePosition(CameraMovement::kForward, step_value);
		}
		if (input->Key(KeyCode::kKeyS) != KeyState::kRelease)
		{
			camera_->UpdatePosition(CameraMovement::kBackward, step_value);
		}
		if (input->Key(KeyCode::kKeyA) != KeyState::kRelease)
		{
			camera_->UpdatePosition(CameraMovement::kLeft, step_value);
		}
		if (input->Key(KeyCode::kKeyD) != KeyState::kRelease)
		{
			camera_->UpdatePosition(CameraMovement::kRight, step_value);
		}
		if (input->Key(KeyCode::kKeyE) != KeyState::kRelease)
		{
			camera_->UpdatePosition(CameraMovement::kUp, step_value);
		}
		if (input->Key(KeyCode::kKeyQ) != KeyState::kRelease)
		{
			camera_->UpdatePosition(CameraMovement::kDown, step_value);
		}

		//// If the event has already been handled by another layer nothing
		//if (e.GetEventHandled() == true) { return; }

		//// Set flag indiciating that event has been handled by current layer
		//e.SetEventHandled(true);
		//if (EventVerticalScroll* event = dynamic_cast<EventVerticalScroll*>(&e)) {
		//	const Camera* camera = scene->GetCamera();
		//	render_camera_->UpdateFOV(event->GetYOffset());
		//}
	}

	void Scene::AddModel(std::shared_ptr<Model> model)
	{
		models_.push_back(model);
	}
}
