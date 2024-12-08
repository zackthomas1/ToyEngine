#pragma once
#include "pch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
		WindowsWindow(const WindowProps& props);
		~WindowsWindow();

		static std::unique_ptr<WindowsWindow> Create(const WindowProps& props = WindowProps());

		// getter/setters 
		unsigned int GetWidth() const;
		unsigned int GetHeight() const; 

		// input processing and call-back
		void ProcessInput();
		void SetFrameBufferSizeCallback(void (*framebuffer_size_callback)(GLFWwindow* window, int width, int height));
		void SetKeyCallback(void (*key_callback)(GLFWwindow* window, int key, int scancode, int action, int mods));
		void SetCursorPositionCallback(static void (cursor_position_callback)(GLFWwindow* window, double xpos, double ypos));
		void SetScrollCallback(void (*scroll_callback)(GLFWwindow* window, double x_offset, double y_offset));

		// state
		void MakeContextCurrent();
		void SetInputMode();
		void SwapBuffers(); 
		void PollEvents();
		bool ShouldClose();
	private:
		void Init(const WindowProps& props);
		void Shutdown();
	private: 
		GLFWwindow* window_;

		struct WindowSettings
		{
			std::string title;
			unsigned int width, height;
		};

		WindowSettings settings_;

	};
}