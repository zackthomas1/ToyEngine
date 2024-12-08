#pragma once
#include "ToyEngine/core.h"
#include "ToyEngine/window.h"

namespace ToyEngine
{
	class Application
	{
	public:
		Application(); 
		virtual ~Application(); 

		void Run();
	private:
		std::unique_ptr<Window> window_;
		static Application* s_instance;
	};

	// defined by client
	Application* CreateApplication();
}