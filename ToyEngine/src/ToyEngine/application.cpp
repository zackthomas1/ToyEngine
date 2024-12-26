#include "pch.h"
#include "ToyEngine/application.h"
#include "ToyEngine/events/event_handler.h"

#include "ToyEngine/services/time_step_glfw.h"
#include "ToyEngine/services/locator.h"

namespace ToyEngine
{
	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		TY_CORE_ASSERT(!s_instance, "Application already exist!")
		s_instance = this;

		// initialize window
		window_ = std::unique_ptr<WindowsWindow>(WindowsWindow::Create());
		window_->SetCommandCallbackFn(Application::EventHandler);

		// initialize time step
		Locator::SetTimeStepProvider(new TimeStepGLFW());

		// create scene
		scene_ = std::make_shared<Scene>();

		// initialize renderer
		renderer_ = std::unique_ptr<Renderer>(Renderer::Create());
		renderer_->SetCamera((*scene_->GetCameras())[0]);
	}

	Application::~Application()
	{
		Locator::DeleteTimeStepProvider();
	}

	void Application::Update(float time_delta)
	{
		scene_->Update(time_delta);
	}

	void Application::Run()
	{
		while (!window_->ShouldClose())
		{
			// update variable time step
			Locator::TimeStepService()->Update();
			TY_CORE_INFO("{}", Locator::TimeStepService()->GetTimeStep()); 

			// handle any user input since the last call
			window_->ProcessInput();

			// advance the game simulation one step
			Update(Locator::TimeStepService()->GetTimeStep());

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