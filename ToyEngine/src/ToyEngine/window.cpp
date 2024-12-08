#include "pch.h"
#include "window.h"

namespace ToyEngine
{
	Window::Window()
	{
		Init();
	}

	Window::~Window()
	{

	}

	void Window::Init()
	{
		TY_CORE_INFO("Window Initialize");

		// Initialize GLFW
		//glfwInit();
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		////glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	#ifdef __APPLE__
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	}

	Window* Window::Create()
	{
		return new Window();
	}
}
