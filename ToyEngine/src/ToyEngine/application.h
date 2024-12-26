#pragma once
#include "core.h"
#include "ToyEngine/events/event.h"
#include "ToyEngine/windows_window.h"
#include "ToyEngine/renderer/renderer.h"



namespace ToyEngine
{
	class Application
	{
	public:
		Application();
		virtual ~Application(); 

		void Update(float time_delta);
		void Run();

	private:
		static void EventHandler(Event& e);
	private:
		std::unique_ptr<WindowsWindow> window_;
		std::unique_ptr<Renderer> renderer_;
		std::shared_ptr<Scene> scene_;
		static Application* s_instance;
	};

	// defined by client
	Application* CreateApplication();
}