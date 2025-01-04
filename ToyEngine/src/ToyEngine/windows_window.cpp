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

	bool WindowsWindow::ShouldClose()
	{
		return glfwWindowShouldClose(window_);
	}

	void WindowsWindow::ProcessInput()
	{
		if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window_, TRUE);
			EventKeyInput key_press(KeyCode::kKeyEscape, KeyState::kPress);
			data_.event_callback(key_press);
		}
		if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
			EventKeyInput key_press(KeyCode::kKeyW, KeyState::kPress);
			data_.event_callback(key_press);
		}
		if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
			EventKeyInput key_press(KeyCode::kKeyS, KeyState::kPress);
			data_.event_callback(key_press);
		}
		if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
			EventKeyInput key_press(KeyCode::kKeyA, KeyState::kPress);
			data_.event_callback(key_press);
		}
		if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
			EventKeyInput key_press(KeyCode::kKeyD, KeyState::kPress);
			data_.event_callback(key_press);
		}
		if (glfwGetKey(window_, GLFW_KEY_E) == GLFW_PRESS) {
			EventKeyInput key_press(KeyCode::kKeyE, KeyState::kPress);
			data_.event_callback(key_press);
		}
		if (glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS) {
			EventKeyInput key_press(KeyCode::kKeyQ, KeyState::kPress);
			data_.event_callback(key_press);
		}
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
		data_.x_mouse_pos = props.x_mouse_pos; 
		data_.y_mouse_pos = props.y_mouse_pos;

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

		// set mouse capture
		//glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
			//TY_CORE_WARN("TODO: implement key call-back function");
		});

		glfwSetScrollCallback(window_, [](GLFWwindow* window, double x_offset, double y_offset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			EventVerticalScroll scroll(y_offset); 
			data.event_callback(scroll);

			//TY_CORE_WARN("TODO: implement scroll call-back function");
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

			//TY_CORE_WARN("TODO: implement cursor pos call-back function");
		});
	}

	void WindowsWindow::Shutdown()
	{
		TY_CORE_TRACE("Shutdown window");
		glfwTerminate();
	}
}