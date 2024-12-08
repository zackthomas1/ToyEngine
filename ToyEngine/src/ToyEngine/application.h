#pragma once
#include "core.h"
#include "windows_window.h"

namespace ToyEngine
{
	class Application
	{
	public:
		Application(); 
		virtual ~Application(); 

		void Run();
	private:
		std::unique_ptr<WindowsWindow> window_;
		static Application* s_instance;
	};

	// defined by client
	Application* CreateApplication();
}