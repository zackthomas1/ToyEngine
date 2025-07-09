#pragma once
#include "ToyEngine/events/event.h"

namespace ToyEngine {
	constexpr int TY_DEFAULT_WINDOW_WIDTH = 800;
	constexpr int TY_DEFAULT_WINDOW_HEIGHT = 600;

	struct WindowProps
	{
		std::string title;
		unsigned int width;
		unsigned int height;
		bool is_mouse_active;
		float x_mouse_pos, y_mouse_pos;

		WindowProps(const std::string& title = "Learn OpenGL",
			unsigned int width = TY_DEFAULT_WINDOW_WIDTH,
			unsigned int height = TY_DEFAULT_WINDOW_HEIGHT,
			bool is_mouse_active = false,
			float x_mouse_pos = TY_DEFAULT_WINDOW_WIDTH / 2.0f,
			float y_mouse_pos = TY_DEFAULT_WINDOW_HEIGHT / 2.0f)
			: title(title), width(width), height(height),
			is_mouse_active(is_mouse_active), x_mouse_pos(x_mouse_pos), y_mouse_pos(y_mouse_pos)
		{
		}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() {}

		static Scope<Window> Create(const WindowProps& props = WindowProps());

		virtual void OnUpdate() = 0;
		
	
		/// <summary>
		/// Sets the callback function that will be invoked when an event occurs in the window.
		/// The callback receives a reference to an Event object, allowing custom event handling logic.
		/// This method must be implemented by derived classes to connect the window's event system
		/// with the application's event processing code.
		/// </summary>
		/// <param name="callback"></param>
		virtual void SetCommandCallbackFn(const EventCallbackFn& callback) = 0;
		float GetAspectRatio() const { return static_cast<float>(data_.width) / static_cast<float>(data_.height); }
		void SetWindowSize(unsigned int width, unsigned int height) { data_.width = width; data_.height = height; }
	protected:
		Window() {}
		Window(const WindowProps& props);
	protected:
		struct WindowData
		{
			std::string title;
			unsigned int width, height;
			EventCallbackFn event_callback;
			bool is_mouse_active;
			float x_mouse_pos, y_mouse_pos;
		};
		WindowData data_;
	};
}