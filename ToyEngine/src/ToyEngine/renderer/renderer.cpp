#include "pch.h"
#include "renderer.h"

#include "ToyEngine/renderer/mesh/primatives/plane.h"
#include "ToyEngine/renderer/mesh/primatives/cube.h"
#include "ToyEngine/renderer/mesh/primatives/triangle.h"

#include <glad/glad.h>

namespace ToyEngine
{
	Renderer::Renderer()
	{
		shader_ = std::make_shared<Shader>("../assets/shaders/1_9_camera.vs", "../assets/shaders/1_9_camera.fs");
	}

	void Renderer::DrawScene(std::shared_ptr<Scene> scene)
	{
        // clear and set screen
        // ------
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// draw each model in the scene
		for (std::shared_ptr<Model> model : *scene->GetSceneModels())
		{
			// activate shader and texture unites
			shader_->Use();
			model->GetMaterial()->SetMaterialUniforms(shader_);
			model->GetMaterial()->ActivateTextureUnits();
		
			// texture related unifroms 
			shader_->SetFloat("u_alpha_tex", 0.5f);
			shader_->SetFloat("u_scale_tex", 1.5f);
			shader_->SetFloat2("u_pos_tex", 0.5f, 0.5f);

			// send camera data to vertex shader
			shader_->SetMat4("u_view", render_camera_.GetViewMatrix());
			shader_->SetMat4("u_projection", render_camera_.GetProjectionMatrix());

			// set model transforms in vertex shader
			shader_->SetMat4("u_model", model->GetModelMatrix());
			
			// draw mesh
			model->GetMesh()->Draw();
		}
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
					render_camera_.UpdatePosition(CameraMovement::kForward, kTimeStep);
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
					render_camera_.UpdatePosition(CameraMovement::kBackward, kTimeStep);
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
					render_camera_.UpdatePosition(CameraMovement::kLeft, kTimeStep);
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
					render_camera_.UpdatePosition(CameraMovement::kRight, kTimeStep);
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
					render_camera_.UpdatePosition(CameraMovement::kUp, kTimeStep);
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
					render_camera_.UpdatePosition(CameraMovement::kDown, kTimeStep);
					break;
				}
				}
				break;
			}
			}
		}
	}

	std::unique_ptr<Renderer> Renderer::Create()
	{
		return std::make_unique<Renderer>();
	}

}