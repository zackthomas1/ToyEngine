#pragma once
#include "core.h"
#include "ToyEngine/event.h"
#include "ToyEngine/windows_window.h"
#include "ToyEngine/renderer/renderer.h"


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
		std::unique_ptr<Renderer> renderer_;
		static Application* s_instance;
	};

	// defined by client
	Application* CreateApplication();
}