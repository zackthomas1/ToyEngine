#include "pch.h"
#include "windows_window.h"

namespace ToyEngine
{
	WindowsWindow::WindowsWindow(const WindowProps& props) : Window(props)
	{
		Init();
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{
		glfwSwapBuffers(window_);
		glfwPollEvents();
	}

	void WindowsWindow::Init()
	{
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

		glfwSetWindowUserPointer(window_, &data_);

		// Uncomment to capture mouse in window and disable mouse visiability  
		//glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	}

	/// <summary>
	/// Set the glfw call back functions for this window.
	/// Sets FramebufferSizeCallback KeyCallback, ScrollCallback, and CursorPosCallback. 
	/// </summary>
	void WindowsWindow::SetCallbackFns()
	{
		glfwSetFramebufferSizeCallback(window_, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			// make sure the viewport matches the new window dimensions; note that width and 
			// height will be significantly larger than specified on retina displays.
			TY_CORE_TRACE("Update Framebuffer size: width - {} height - {}", width, height);
			glViewport(0, 0, width, height);
			
			EventWindowResize window_resize(width, height);
			data.event_callback(window_resize);
		
		});


		glfwSetWindowCloseCallback(window_, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			EventApplicationClose app_close = EventApplicationClose();
			data.event_callback(app_close);
		});

		glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			//TY_CORE_TRACE("Key: {} Action: {}", key, action);
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
				EventApplicationClose app_close = EventApplicationClose();
				data.event_callback(app_close);
			}
			else if (action != GLFW_RELEASE) {
				glfwSetWindowShouldClose(window, TRUE);
				EventKeyInput key_press (static_cast<eKeyCode>(key), static_cast<eKeyState>(action));
				data.event_callback(key_press);
			}
		});

		glfwSetScrollCallback(window_, [](GLFWwindow* window, double x_offset, double y_offset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			
			EventVerticalScroll scroll(y_offset); 
			data.event_callback(scroll);
		});

		glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double xpos, double ypos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			
			float x_current_pos = static_cast<float>(xpos);
			float y_current_pos = static_cast<float>(ypos);

			if (!data.is_mouse_active)
			{
				data.x_mouse_pos = x_current_pos; 
				data.y_mouse_pos = y_current_pos;
				data.is_mouse_active = true;
			}
			float x_offset = x_current_pos - data.x_mouse_pos; 
			float y_offset = y_current_pos - data.y_mouse_pos;
			
			data.x_mouse_pos = x_current_pos;
			data.y_mouse_pos = y_current_pos;

			EventCursorPos cursor_pos(x_offset, y_offset);
			data.event_callback(cursor_pos);
		});
	}

	void WindowsWindow::Shutdown()
	{
		TY_CORE_INFO("Window shutdown");
		glfwTerminate();
	}
}