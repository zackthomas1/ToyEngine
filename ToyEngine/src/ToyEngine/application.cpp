#include "pch.h"
#include "ToyEngine/application.h"
#include "ToyEngine/commands/command.h"

namespace ToyEngine
{
	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		//
		TY_CORE_ASSERT(!s_instance, "Application already exist!")
		s_instance = this;

		//
		window_ = std::unique_ptr<WindowsWindow>(WindowsWindow::Create());
		window_->SetCommandCallbackFn(Application::EventHandler);

		//
		is_running_ = true;
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (is_running_)
		{
			window_->ProcessInput(0.0f);

			// render
			// ------
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

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