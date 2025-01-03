#include "pch.h"
#include "renderer.h"

#include "ToyEngine/renderer/render_api.h"
#include "ToyEngine/renderer/mesh/primatives/plane.h"
#include "ToyEngine/renderer/mesh/primatives/cube.h"
#include "ToyEngine/renderer/mesh/primatives/triangle.h"

#include "ToyEngine/services/locator.h"

#include <glad/glad.h>

namespace ToyEngine
{
	Renderer::Renderer()
	{
		shader_ = new Shader("../assets/shaders/1_9_camera.vs", "../assets/shaders/1_9_camera.fs");
	}

	Renderer::~Renderer()
	{
		delete shader_;
	}

	void Renderer::DrawScene(std::shared_ptr<Scene> scene)
	{
        // clear and set screen
        // ------
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// draw each model in the scene
		for (std::shared_ptr<Model> model : *scene->GetModels())
		{
			// activate shader and texture unites
			RenderAPI::ShaderUse(shader_);
			model->GetMaterial()->SetMaterialUniforms(shader_);
			model->GetMaterial()->ActivateTextureUnits();
		
			// texture related unifroms 
			RenderAPI::SetFloat(shader_, "u_alpha_tex", 0.5f);
			RenderAPI::SetFloat(shader_, "u_scale_tex", 1.5f);
			//shader_->SetFloat("u_alpha_tex", 0.5f);
			//shader_->SetFloat("u_scale_tex", 1.5f);
			shader_->SetFloat2("u_pos_tex", 0.5f, 0.5f);

			// send camera data to vertex shader
			shader_->SetMat4("u_view", render_camera_->GetViewMatrix());
			shader_->SetMat4("u_projection", render_camera_->GetProjectionMatrix());

			// set model transforms in vertex shader
			shader_->SetMat4("u_model", model->GetModelMatrix());
			
			// draw mesh
			model->GetMesh()->Draw();
		}
	}

	void Renderer::OnEvent(Event& e)
	{
		if(EventVerticalScroll* event = dynamic_cast<EventVerticalScroll*>(&e))
		{
			render_camera_->UpdateFOV(event->GetYOffset());
		}

		if (EventCursorPos* event = dynamic_cast<EventCursorPos*>(&e))
		{
			//TY_CORE_INFO("EventCursorPos: x_offset-{} y_offset-{}", event->GetXOffset(), event->GetYOffset());
			render_camera_->UpdateLookDirection(event->GetXOffset(), event->GetYOffset());
		}

		float step_value = Locator::TimeStepService()->GetTimeStep();
		if(EventKeyInput* event = dynamic_cast<EventKeyInput*>(&e))
		{
			switch (event->GetKeyCode())
			{
			case KeyCode::kKeyW:
			{
				switch (event->GetKeyState())
				{
				case KeyState::kPress:
				{
					//TY_CORE_TRACE("OnNotify - Camera Forward");
					render_camera_->UpdatePosition(CameraMovement::kForward, step_value);
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
					//TY_CORE_TRACE("OnNotify - Camera Backward");
					render_camera_->UpdatePosition(CameraMovement::kBackward, step_value);
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
					//TY_CORE_TRACE("OnNotify - Camera Left");
					render_camera_->UpdatePosition(CameraMovement::kLeft, step_value);
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
					//TY_CORE_TRACE("OnNotify - Camera Right");
					render_camera_->UpdatePosition(CameraMovement::kRight, step_value);
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
					//TY_CORE_TRACE("OnNotify - Camera Up");
					render_camera_->UpdatePosition(CameraMovement::kUp, step_value);
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
					//TY_CORE_TRACE("OnNotify - Camera Down");
					render_camera_->UpdatePosition(CameraMovement::kDown, step_value);
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