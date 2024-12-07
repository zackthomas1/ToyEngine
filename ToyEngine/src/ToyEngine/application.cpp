#include "pch.h"
#include "ToyEngine/application.h"

namespace ToyEngine
{
	Application* Application::s_instance = nullptr;

	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (true)
		{
			TY_CORE_INFO("running");
		}
	}


}