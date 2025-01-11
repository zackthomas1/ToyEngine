#include "pch.h"
#include "layer.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace ToyEngine
{
	ImGuiLayer::ImGuiLayer() : io_(ImGui::GetIO())
	{

	}

	void ImGuiLayer::Update(float time_delta)
	{
		// ImGui
		// ------------------------------

		// Start the Dear Imgui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//
		CreateWindows();
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		if (io_.WantCaptureMouse == true || io_.WantCaptureKeyboard == true)
		{
			e.SetEventHandled(true);
		}
	}
}