#pragma once
#include "pch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ToyEngine/event.h"

namespace ToyEngine
{
	struct WindowProps
	{
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProps(const std::string& title = "Learn OpenGL",
			unsigned int width = 800,
			unsigned int height = 600)
			: title(title), width(width), height(height) {
		}
	};

	class WindowsWindow
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		WindowsWindow(const WindowProps& props);
		~WindowsWindow();

		static std::unique_ptr<WindowsWindow> Create(const WindowProps& props = WindowProps());

		void SetCommandCallbackFn(const EventCallbackFn& callback) { data_.event_callback = callback; }

		void ProcessInput(float time_step);
		
		void SetInputMode();
		void SwapBuffers(); 
		void PollEvents();
		
	private:
		void Init(const WindowProps& props);
		void SetCallbackFns();
		void Shutdown();
	private: 
		GLFWwindow* window_;

		struct WindowSettings
		{
			std::string title;
			unsigned int width, height;
			EventCallbackFn event_callback;
		};

		WindowSettings data_;
	};
}