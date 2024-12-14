#include "pch.h"
#include "ToyEngine/application.h"
#include "ToyEngine/commands/command.h"

namespace ToyEngine
{
	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		TY_CORE_ASSERT(!s_instance, "Application already exist!")
		s_instance = this;

		window_ = std::unique_ptr<WindowsWindow>(WindowsWindow::Create());
		window_->SetCommandCallbackFn(Application::EventHandler);
		is_running_ = true;

		std::shared_ptr<Scene> scene = std::make_shared<Scene>();

		renderer_ = std::unique_ptr<Renderer>(Renderer::Create(scene));
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (is_running_)
		{
			// TODO: Implement function that calculates the time 
			// difference between two frames rendering
			float time_delta = 0.0f;
			window_->ProcessInput(time_delta);

			renderer_->DrawScene();

			window_->SwapBuffers();
			window_->PollEvents();
		}
	}

	void Application::EventHandler(Event& e)
	{
		if (EventKeyPress* event = dynamic_cast<EventKeyPress*>(&e))
		{
			switch (event->GetKeyCode())
			{
			case KeyCode::key_escape:
			{
				CommandWindowClose command(&(s_instance->is_running_));
				command.Execute();
				break;
			}
			case KeyCode::key_w:
			{
				CommandCameraForward command = CommandCameraForward();
				command.Execute();
				break;
			}
			case KeyCode::key_s:
			{
				CommandCameraBackwards command = CommandCameraBackwards();
				command.Execute();
				break;
			}
			default:
			{}
			}
		}
		else if (EventKeyRelease* event = dynamic_cast<EventKeyRelease*>(&e))
		{

		}

	}
}