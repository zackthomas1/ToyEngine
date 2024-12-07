#include <toy_engine.h>

class Sandbox : public ToyEngine::Application
{
public:
	Sandbox()
	{
		TY_INFO("Initialize application");
	}
	~Sandbox()
	{

	}
};

ToyEngine::Application* ToyEngine::CreateApplication()
{
	return new Sandbox();
}
