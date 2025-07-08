#include "pch.h"
#include "layer.h"
#include "ToyEngine/application.h"
#include "ToyEngine/platform/windows/windows_window.h"
// imgui
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace ToyEngine
{
	ImGuiLayer::ImGuiLayer()
	{

	}

	void ImGuiLayer::OnAttach()
	{
		TY_CORE_ASSERT(ImGui::GetCurrentContext() == nullptr, "ImGui - Already been initialized context.");
		
		Application &app = Application::Get();
		GLFWwindow* window = dynamic_cast<WindowsWindow*>(&Application::Get().GetWindow())->GetGLFWWindow();

		// Setup Dear ImGui context
		// -------------------------
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io_ = ImGui::GetIO(); (void)io_;
		io_.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;		// Enable Keyboard Controls
		io_.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;		// Enable Gamepad Controls 
		io_.ConfigFlags |= ImGuiConfigFlags_DockingEnable;			// Enable Docking 
		io_.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;		// Enable Multi-Viewport / Platform Windows

		// Setup Dear ImGui style 
		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io_.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 10.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer backends 
		TY_CORE_ASSERT(io_.BackendPlatformUserData == nullptr, "ImGui - Already initialized a platform backend.");
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330 core");
		// -------------------------
	}

	void ImGuiLayer::OnDetatch()
	{
		// Cleanup imgui
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	void ImGuiLayer::BeginDraw()
	{
		// ImGui
		// ------------------------------
		// Start the Dear Imgui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::EndDraw()
	{
		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		ImGuiIO& io_ = ImGui::GetIO(); (void)io_;
		if (io_.WantCaptureMouse == true || io_.WantCaptureKeyboard == true)
		{
			e.SetEventHandled(true);
		}
	}
}