#include "pch.h"
#include "ToyEngine/application.h"
#include "ToyEngine/events/event_handler.h"

namespace ToyEngine
{
	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		TY_CORE_ASSERT(!s_instance, "Application already exist!")
		s_instance = this;

		window_ = std::unique_ptr<WindowsWindow>(WindowsWindow::Create());
		window_->SetCommandCallbackFn(Application::EventHandler);

		std::shared_ptr<Scene> scene = std::make_shared<Scene>();
		renderer_ = std::unique_ptr<Renderer>(Renderer::Create(scene));
	}

	Application::~Application()
	{

	}
	void Application::Update(float time_delta)
	{
		//renderer_->UpdateScene();
	}

	void Application::Run()
	{
		while (!window_->ShouldClose())
		{
			// TODO: Implement variable time step
			float time_delta = 1.0f;

			// handle any user input since the last call
			//ProcessInput();
			window_->ProcessInput();

			//advance the game simulation one step
			Update(time_delta);

			// draw the game
			renderer_->DrawScene();

			window_->SwapBuffers();
			window_->PollEvents();
		}
	}

	void Application::EventHandler(Event& e)
	{
		s_instance->renderer_->OnNotify(e);
	}
}