#include <toy_engine.h>

class EditorLayer : public ToyEngine::Layer 
{
	virtual void OnImGuiRender() 
	{
		// Define the GUI windows
		// ------------------------------
		ImGuiIO& io_ = ImGui::GetIO(); (void)io_;

		// Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! 
		// You can browse its code to learn more about Dear ImGui!).
		ImGui::ShowDemoWindow();

		// Show simple window
		ImGui::Begin("Hello, World");
		ImGui::Text("This is some useful text");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io_.Framerate, io_.Framerate);
		ImGui::End();
	}
};

class Editor : public ToyEngine::Application
{
public:
	Editor()
	{
		TY_INFO("Initialize application");
		PushLayer(new EditorLayer());
	}
	~Editor()
	{

	}
};

ToyEngine::Application* ToyEngine::CreateApplication()
{
	return new Editor();
}
