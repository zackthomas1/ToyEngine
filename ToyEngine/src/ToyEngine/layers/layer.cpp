#include "pch.h"
#include "layer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace ToyEngine
{
	void ImGuiLayer::Update(float time_delta)
	{
		// ImGui
		// ------------------------------
		ImGuiIO& io = ImGui::GetIO(); (void)io;

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
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
		// ------------------------------
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		if (io.WantCaptureMouse == true || io.WantCaptureKeyboard == true)
		{
			e.SetEventHandled(true);
		}
	}
}