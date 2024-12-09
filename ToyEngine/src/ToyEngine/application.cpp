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
		window_->MakeContextCurrent();
		window_->SetCommandCallbackFn(InputHandler::ExecuteCommand);

		//
		is_running_ = true;

		// GLAD: load all OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			TY_CORE_ERROR("Failed to intialize GLAD");
			window_ = nullptr;
		}

		// Set rendering window size 
		glViewport(0, 0, window_->GetWidth(), window_->GetHeight());
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (!window_->ShouldClose())
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


}