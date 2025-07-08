#pragma once
#include "ToyEngine/events/event.h"

namespace ToyEngine {

	struct WindowProps
	{
		std::string title;
		unsigned int width;
		unsigned int height;
		bool is_mouse_active;
		float x_mouse_pos, y_mouse_pos;

		WindowProps(const std::string& title = "Learn OpenGL",
			unsigned int width = 800,
			unsigned int height = 600,
			bool is_mouse_active = false,
			float x_mouse_pos = 800.0f / 2.0f,
			float y_mouse_pos = 600.0f / 2.0f)
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
		virtual void SetCommandCallbackFn(const EventCallbackFn& callback) = 0;
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