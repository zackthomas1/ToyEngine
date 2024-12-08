#include "pch.h"
#include "windows_window.h"

namespace ToyEngine
{
	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	std::unique_ptr<WindowsWindow> WindowsWindow::Create(const WindowProps& props)
	{
		return std::make_unique<WindowsWindow>(props);
	}

	unsigned int WindowsWindow::GetWidth() const
	{
		return settings_.width;
	}

	unsigned int WindowsWindow::GetHeight() const
	{
		return settings_.height;
	}

	void WindowsWindow::ProcessInput()
	{
		if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
		{
			TY_CORE_INFO("Key_Escape: Press");
			glfwSetWindowShouldClose(window_, true);
		}
	}

	void WindowsWindow::SetFrameBufferSizeCallback(void(*framebuffer_size_callback)(GLFWwindow* window, int width, int height))
	{
		glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback); 	// set call-back function for window resize
	}

	void WindowsWindow::SetKeyCallback(void(*key_callback)(GLFWwindow* window, int key, int scancode, int action, int mods))
	{
		glfwSetKeyCallback(window_, key_callback);								// set call-back function key input
	}

	void WindowsWindow::SetCursorPositionCallback(void(cursor_position_callback)(GLFWwindow* window, double xpos, double ypos))
	{
		glfwSetCursorPosCallback(window_, cursor_position_callback);			// set call-back function cursor pos input
	}

	void WindowsWindow::SetScrollCallback(void(*scroll_callback)(GLFWwindow* window, double x_offset, double y_offset))
	{
		glfwSetScrollCallback(window_, scroll_callback);
	}

	void WindowsWindow::MakeContextCurrent()
	{
		glfwMakeContextCurrent(window_);
	}

	void WindowsWindow::SetInputMode()
	{
		glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);		// capture mouse cursor
	}

	void WindowsWindow::SwapBuffers()
	{
		glfwSwapBuffers(window_);
	}

	void WindowsWindow::PollEvents()
	{
		glfwPollEvents();
	}

	bool WindowsWindow::ShouldClose()
	{
		return static_cast<bool>(glfwWindowShouldClose(window_));
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		settings_.title = props.title;
		settings_.width = props.width;
		settings_.height = props.height;

		// GLFW: initialize and configure
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		// GLFW: window object creation
		window_ = glfwCreateWindow(settings_.width, settings_.height, settings_.title.c_str(), NULL, NULL);
		if (window_ == NULL)
		{
			TY_CORE_ERROR("Failed to create GLFW window");
			glfwTerminate();
			window_ = nullptr;
		}
	}
	void WindowsWindow::Shutdown()
	{
		glfwTerminate();
	}
}