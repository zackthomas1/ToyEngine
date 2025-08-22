#pragma once
#include "ToyEngine/event.h"

struct GLFWwindow;

namespace ToyEngine {
	struct WindowProps
	{
		std::string title;
		unsigned int width;
		unsigned int height;
		bool is_mouse_active;
		float x_mouse_pos, y_mouse_pos;

		WindowProps(const std::string& title = "Editor",
			unsigned int width = TY_DEFAULT_WINDOW_WIDTH,
			bool is_mouse_active = false,
			unsigned int height = TY_DEFAULT_WINDOW_HEIGHT,
			float x_mouse_pos = TY_DEFAULT_WINDOW_WIDTH / 2.0f,
			float y_mouse_pos = TY_DEFAULT_WINDOW_HEIGHT / 2.0f)
				: title(title), is_mouse_active(is_mouse_active),
				width(width), height(height),
				x_mouse_pos(x_mouse_pos), y_mouse_pos(y_mouse_pos)
		{}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() {}

		static Scope<Window> Create(const WindowProps& props = WindowProps());

		virtual void OnUpdate() = 0;
		virtual void* GetNativeWindow() const = 0;
		//sets the callback function that will be invoked when an event occurs in the window.
		//the callback receives a reference to an event object, allowing custom event handling logic.
		//this method must be implemented by derived classes to connect the window's event system
		//with the application's event processing code.
		virtual void SetCommandCallbackFn(const EventCallbackFn& callback) = 0;
		virtual void SetCursorPos(float xpos, float ypos) = 0;

		int GetWidth() const { return data_.width; }
		int GetHeight() const { return data_.height; }
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

	class NullWindow : public Window
	{
		virtual void OnUpdate() override {}
		virtual void* GetNativeWindow() const override { return nullptr; }
		virtual void SetCommandCallbackFn(const EventCallbackFn& callback) override { data_.event_callback = callback; }
		virtual void SetCursorPos(float xpos, float ypos) override {}
	};

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props = WindowProps());
		~WindowsWindow();

		virtual void OnUpdate() override;
		virtual void* GetNativeWindow() const override { return window_; }
		virtual void SetCommandCallbackFn(const EventCallbackFn& callback) override { data_.event_callback = callback; }
		virtual void SetCursorPos(float xpos, float ypos) override;

	private:
		void Init();
		void SetCallbackFns();
		void Shutdown();

	private:
		GLFWwindow* window_;
	};
}