#include "pch.h"
#include "gui_renderer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace ToyEngine
{
	void GuiRenderer::Init(GLFWwindow* window)
	{
		TY_CORE_ASSERT(ImGui::GetCurrentContext() == nullptr, "ImGui - Already been initialized context.");

		// Setup Dear ImGui context
		// -------------------------
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io_ = ImGui::GetIO(); (void)io_;
		io_.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// Enable Keyboard Controls
		io_.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	// Enable Gamepad Controls 
		io_.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		// Enable Docking 
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

	void GuiRenderer::DrawGui()
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

	void GuiRenderer::Delete()
	{
		// Cleanup imgui
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}
