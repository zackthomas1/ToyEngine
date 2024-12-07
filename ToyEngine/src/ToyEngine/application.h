#pragma once
#include "ToyEngine/core.h"
#include "log.h"

namespace ToyEngine
{
	class Application
	{
	public:
		Application(); 
		virtual ~Application(); 

		void Run();
	private:
		static Application* s_instance;
	};

	// defined by client
	Application* CreateApplication();
}