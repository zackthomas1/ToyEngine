#include <toy_engine.h>

#include "imgui.h"


class EditorGui : public ToyEngine::ImGuiLayer
{
public:
	void CreateWindows()
	{
		// Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! 
		// You can browse its code to learn more about Dear ImGui!).
		ImGui::ShowDemoWindow();

		// Show simple window
		ImGui::Begin("Hello, World");
		ImGui::Text("This is some useful text");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io_.Framerate, io_.Framerate);
		ImGui::End();
		// ------------------------------
	}
};

class Editor : public ToyEngine::Application
{
public:
	Editor()
	{
		TY_INFO("Initialize application");
		
		// Create application layers and to layer manager
		ToyEngine::LayerManager::AddLayer(new EditorGui());
	}
	~Editor()
	{

	}
};

ToyEngine::Application* ToyEngine::CreateApplication()
{
	return new Editor();
}
