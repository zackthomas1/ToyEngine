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

	void WindowsWindow::ProcessInput(float time_step)
	{
		if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			EventKeyPress key_press(key_escape);
			data_.event_callback(key_press);
		}
		if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
			EventKeyPress key_press(key_w);
			data_.event_callback(key_press);
		}
		if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
			EventKeyPress key_press(key_s);
			data_.event_callback(key_press);
		}
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

	void WindowsWindow::Init(const WindowProps& props)
	{
		data_.title = props.title;
		data_.width = props.width;
		data_.height = props.height;

		// GLFW: initialize and configure
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		// GLFW: window object creation
		window_ = glfwCreateWindow(data_.width, data_.height, data_.title.c_str(), NULL, NULL);
		if (window_ == NULL)
		{
			TY_CORE_ERROR("Failed to create GLFW window");
			glfwTerminate();
			window_ = nullptr;
		}
		
		glfwMakeContextCurrent(window_);
		SetCallbackFns();

		// GLAD: load all OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			TY_CORE_ERROR("Failed to intialize GLAD");
			window_ = nullptr;
		}

		// Set rendering window size 
		glViewport(0, 0, data_.width, data_.height);
	}

	void WindowsWindow::SetCallbackFns()
	{
		glfwSetFramebufferSizeCallback(window_, [](GLFWwindow* window, int width, int height) {
			// make sure the viewport matches the new window dimensions; note that width and 
			// height will be significantly larger than specified on retina displays.
			TY_CORE_TRACE("Update Framebuffer size: width - {} height - {}", width, height);
			glViewport(0, 0, width, height);
		});

		glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			TY_CORE_WARN("TODO: implement key call-back function");
		});							// set call-back function key input

		glfwSetScrollCallback(window_, [](GLFWwindow* window, double x_offset, double y_offset) {
			TY_CORE_WARN("TODO: implement scroll call-back function");
		});

		glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double xpos, double ypos) {
			TY_CORE_WARN("TODO: implement cursor pos call-back function");
		});			// set call-back function cursor pos input
	}

	void WindowsWindow::Shutdown()
	{
		TY_CORE_TRACE("Shutdown window");
		glfwTerminate();
	}
}