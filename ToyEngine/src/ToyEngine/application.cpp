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

		scene_ = std::make_shared<Scene>();

		renderer_ = std::unique_ptr<Renderer>(Renderer::Create());
		renderer_->SetCamera((*scene_->GetCameras())[0]);
	}

	Application::~Application()
	{

	}

	void Application::Update(float time_delta)
	{
		scene_->Update(time_delta);
	}

	void Application::Run()
	{
		float last_time = glfwGetTime(); // Time of last frame

		while (!window_->ShouldClose())
		{
			// variable time step
			float current_time = glfwGetTime();
			float time_delta = current_time - last_time;
			last_time = current_time;

			// handle any user input since the last call
			window_->ProcessInput();

			// advance the game simulation one step
			Update(time_delta);

			// draw the game
			renderer_->DrawScene(scene_);

			//
			window_->SwapBuffers();
			window_->PollEvents();
		}
	}

	void Application::EventHandler(Event& e)
	{
		s_instance->renderer_->OnEvent(e);
	}
}