#pragma once
#include "core.h"
#include "ToyEngine/event.h"
#include "ToyEngine/window.h"
#include "ToyEngine/layers/layer_stack.h"

namespace ToyEngine
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		// Delete copy/move to prevent reassignment
		// Enforce Singleton pattern
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;
		
		/// <summary>
		/// Starts and manages the main application loop.
		/// Continuously updates the window, processes events, and updates all layers
		/// until the application is closed.
		/// </summary>
		void Run();
		
		/// <summary>
		/// Handles incoming events and dispatches them to the appropriate layers.
		/// This function is called whenever an event occurs (e.g., input, window events).
		/// It propagates the event through the layer stack in reverse order (from topmost to bottom),
		/// allowing each layer to handle or consume the event as needed.
		/// </summary>
		/// <param name="e">Reference to the event object to be processed.</param>
		void OnEvent(Event& e);
		
		/// <summary>
		/// Adds a new layer to the application. Layers are used for core engine logic and are updated and rendered in order.
		/// The layer is inserted below overlays and above previously added layers.
		/// </summary>
		/// <param name="layer">Pointer to the Layer to be added.</param>
		void PushLayer(Layer *layer);

		/// <summary>
		/// Adds a new overlay to the application. Overlays are rendered and updated after all regular layers.
		/// Typically used for UI or debug panels.
		/// </summary>
		/// <param name="layer">Pointer to the Layer to be added as an overlay.</param>
		void PushOverlay(Layer *layer);

		ImGuiLayer* GetImGuiLayer() { return imGuiLayer_; }

		// Accessors return const references to prevent modification
		inline static Window& AccessWindow() { return *Application::s_instance->window_; }
		inline static Application& Get() { return *s_instance; }	
	private:
		bool OnClose(EventApplicationClose& e);
		bool OnResize(EventWindowResize& e);
	private:
		Scope<Window> window_;
		ImGuiLayer *imGuiLayer_;	// imGuiLayer is owned by the layerStack_. Deleted by layerStack_ destructor
		LayerStack layerStack_;
		bool isRunning_ = true;

		static Application *s_instance;
	};

	/// <summary>
	/// This function is defined in the client application
	/// </summary>
	Application* CreateApplication();
}