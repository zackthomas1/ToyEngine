#include <toy_engine.h>



class Sandbox : public ToyEngine::Application
{
public:
	Sandbox()
	{
		TY_INFO("Initialize application");
		
		// Create application layers and to layer manager
		ToyEngine::LayerManager::AddLayer(new ToyEngine::ImGuiLayer());
	}
	~Sandbox()
	{

	}
};

ToyEngine::Application* ToyEngine::CreateApplication()
{
	return new Sandbox();
}
