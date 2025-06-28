#pragma once
#include "core.h"
#include "ToyEngine/events/event.h"
#include "ToyEngine/renderer/renderer.h"
#include "ToyEngine/windows_window.h"
#include "ToyEngine/layers/layer_stack.h"

namespace ToyEngine
{
	class Application
	{
	public:
		Application();
		virtual ~Application(); 
		
		/// <summary>
		/// Contains main game-loop. Continously runs until application closed
		/// </summary>
		void Run();
		/// <summary>
		/// 
		/// </summary>
		/// <param name="e"></param>
		void OnEvent(Event& e);
		void PushLayer(Layer *layer);
		void PushOverlay(Layer *layer);
		inline WindowsWindow& GetWindow() { return *window_; }
		inline static Application& Get() { return *s_instance; }
	private:
		std::unique_ptr<WindowsWindow> window_;
		ImGuiLayer *imGuiLayer_;
		LayerStack layerStack_;

		std::unique_ptr<Renderer> renderer_;
		std::shared_ptr<Scene> scene_;

		static Application *s_instance;
	};

	/// <summary>
	/// This function is defined in the client application
	/// </summary>
	Application* CreateApplication();
}