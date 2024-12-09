#pragma once
#include "core.h"
#include "ToyEngine/windows_window.h"
#include "ToyEngine/commands/input_handler.h"


namespace ToyEngine
{
	class Application
	{
	public:
		Application();
		virtual ~Application(); 

		void Run();
	private:
		bool is_running_;
		std::unique_ptr<WindowsWindow> window_;
		static Application* s_instance;
	};

	// defined by client
	Application* CreateApplication();
}