#pragma once
#include "core.h"
#include "ToyEngine/event.h"
#include "ToyEngine/windows_window.h"


namespace ToyEngine
{
	class Application
	{
	public:
		Application();
		virtual ~Application(); 

		void Run();

		static void EventHandler(Event& e);
	private:
		bool is_running_;
		std::unique_ptr<WindowsWindow> window_;
		static Application* s_instance;
	};

	// defined by client
	Application* CreateApplication();
}