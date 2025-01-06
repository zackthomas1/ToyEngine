#include "pch.h"
#include "ToyEngine/application.h"
#include "ToyEngine/events/event_handler.h"

#include "ToyEngine/services/time_step_glfw.h"
#include "ToyEngine/services/locator.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace ToyEngine
{
	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		TY_CORE_ASSERT(!s_instance, "Application already exist!")
		s_instance = this;

		// Initialize window
		window_ = std::unique_ptr<WindowsWindow>(WindowsWindow::Create());
		window_->SetCommandCallbackFn(Application::EventHandler);

		// Setup Dear ImGui context
		// -------------------------
		IMGUI_CHECKVERSION(); 
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	// Enable Gamepad Controls 
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		// Enable Docking 
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;	// Enable Multi-Viewport / Platform Windows

		// Setup Dear ImGui style 
		ImGui::StyleColorsDark(); 

		ImGuiStyle& style = ImGui::GetStyle(); 
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 10.0f; 
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer backends 
		ImGui_ImplGlfw_InitForOpenGL(window_->GetGLFWWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 330 core");
		// -------------------------

		// initialize time step
		Locator::SetTimeStepProvider(new TimeStepGLFW());

		// create scene
		scene_ = std::make_shared<Scene>();

		// initialize renderer
		renderer_ = std::unique_ptr<Renderer>(Renderer::Create());
		renderer_->SetRenderCamera(scene_->GetCameras()[0]);
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
			// Update variable time step
			Locator::TimeStepService()->Update();

			// Handle any user input since the last call
			window_->ProcessInput();

			// ImGui
			// ------------------------------
			// Start the Dear Imgui frame
			ImGui_ImplOpenGL3_NewFrame(); 
			ImGui_ImplGlfw_NewFrame(); 
			ImGui::NewFrame();

			// Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! 
			// You can browse its code to learn more about Dear ImGui!).
			ImGui::ShowDemoWindow();

			// Show simple window
			ImGui::Begin("Hello, World"); 
			ImGui::Text("This is some useful text"); 
			//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
			// ------------------------------


			// Advance the game simulation one step
			Update(Locator::TimeStepService()->GetTimeStep());

			// Draw the game
			ImGui::Render();

			renderer_->DrawScene(scene_);

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			//
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}
			window_->SwapBuffers();
			window_->PollEvents();
		}
	}

	void Application::EventHandler(Event& e)
	{
		s_instance->renderer_->OnEvent(e);
		s_instance->scene_->OnEvent(e);
	}
}