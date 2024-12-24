#include "pch.h"
#include "renderer.h"

#include "ToyEngine/renderer/mesh/primatives/plane.h"
#include "ToyEngine/renderer/mesh/primatives/cube.h"
#include "ToyEngine/renderer/mesh/primatives/triangle.h"

#include <glad/glad.h>

namespace ToyEngine
{
	// TODO: Remove cube position array.
	// This was for early testing
	glm::vec3 cube_positions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	Renderer::Renderer(std::shared_ptr<Scene> scene) : scene_(scene)
	{
		std::shared_ptr<Shader> shader = std::make_shared<Shader>("../assets/shaders/1_9_camera.vs", "../assets/shaders/1_9_camera.fs");
		std::shared_ptr<Cube> cube = std::make_shared<Cube>();
		std::shared_ptr<Material> material = std::make_shared<Material>();
		
		model_ = Model(shader, cube, material);

		model_.GetShader()->Use();
		model_.GetShader()->SetFloat3("pickedColor", 1.0f, 0.0f, 0.0f);
		model_.SetMaterialUniforms();

		camera_ = FlyCamera();
	}

	void Renderer::DrawScene()
	{
        // clear and set screen
        // ------
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		Draw(model_);
		scene_->DrawScene();
	}

	void Renderer::OnNotify(Event& e)
	{
		const float kTimeStep = 0.01f;
		if (EventKeyInput* event = dynamic_cast<EventKeyInput*>(&e))
		{
			switch (event->GetKeyCode())
			{
			case KeyCode::kKeyW:
			{
				switch (event->GetKeyState())
				{
				case KeyState::kPress:
				{
					TY_CORE_TRACE("OnNotify - Camera Forward");
					camera_.UpdatePosition(CameraMovement::kForward, kTimeStep);
					break;
				}
				}
				break;
			}
			case KeyCode::kKeyS:
			{
				switch (event->GetKeyState())
				{
				case KeyState::kPress:
				{
					TY_CORE_TRACE("OnNotify - Camera Backward");
					camera_.UpdatePosition(CameraMovement::kBackward, kTimeStep);
					break;
				}
				}
				break;
			}
			case KeyCode::kKeyA:
			{
				switch (event->GetKeyState())
				{
				case KeyState::kPress:
				{
					TY_CORE_TRACE("OnNotify - Camera Left");
					camera_.UpdatePosition(CameraMovement::kLeft, kTimeStep);
					break;
				}
				}
				break;
			}
			case KeyCode::kKeyD:
			{
				switch (event->GetKeyState())
				{
				case KeyState::kPress:
				{
					TY_CORE_TRACE("OnNotify - Camera Right");
					camera_.UpdatePosition(CameraMovement::kRight, kTimeStep);
					break;
				}
				}
				break;
			}
			case KeyCode::kKeyE:
			{
				switch (event->GetKeyState())
				{
				case KeyState::kPress:
				{
					TY_CORE_TRACE("OnNotify - Camera Up");
					camera_.UpdatePosition(CameraMovement::kUp, kTimeStep);
					break;
				}
				}
				break;
			}
			case KeyCode::kKeyQ:
			{
				switch (event->GetKeyState())
				{
				case KeyState::kPress:
				{
					TY_CORE_TRACE("OnNotify - Camera Down");
					camera_.UpdatePosition(CameraMovement::kDown, kTimeStep);
					break;
				}
				}
				break;
			}
			}
		}
	}

	void Renderer::Draw(Model& model)
	{
		// activate shader and texture unites
		model.GetShader()->Use();
		model.GetMaterial()->ActivateTextureUnits();

		// texture related unifroms 
		model.GetShader()->SetFloat("u_alpha_tex", 0.5f);
		model.GetShader()->SetFloat("u_scale_tex", 1.5f);
		model.GetShader()->SetFloat2("u_pos_tex", 0.5f, 0.5f);

		// send camera data to vertex shader
		model.GetShader()->SetMat4("u_view", camera_.GetViewMatrix()); 
		model.GetShader()->SetMat4("u_projection", camera_.GetProjectionMatrix());

		// draw call cubes
		for (int i = 0; i < sizeof(cube_positions) / sizeof(glm::vec3); i++)
		{
			// Update model matrix
			glm::mat4 transforms = glm::mat4(1.0f);
			transforms = glm::translate(transforms, cube_positions[i]);
			float angle = 20.0f * i;
			transforms = glm::rotate(transforms, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			model.SetModelMatrix(transforms);

			model.GetShader()->SetMat4("u_model", model.GetModelMatrix());
			model.GetMesh()->Draw();
		}
	}

	std::unique_ptr<Renderer> Renderer::Create(std::shared_ptr<Scene> scene)
	{
		return std::make_unique<Renderer>(scene);
	}
}