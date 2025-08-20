#include "pch.h"
#include "ToyEngine/application.h"
#include "ToyEngine/services/input_poll.h"
#include "ToyEngine/platform/windows/windows_window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ToyEngine {
	InputPollGLFW::InputPollGLFW(Window* window)
	{
		TY_CORE_ASSERT(window_ = dynamic_cast<WindowsWindow*>(window), "Failed cast to WindowsWindow/");
	}

	void InputPollGLFW::Init()
	{
		TY_CORE_INFO("GLFW input poll provider");
	}

	eKeyState InputPollGLFW::Key(eKeyCode key) const
	{
		GLFWwindow* window = window_->GetGLFWWindow();
		TY_CORE_ASSERT(window, "GLFWwindow is Null. Initialize windowing before calling input polling service provider.");
		eKeyState key_state = static_cast<eKeyState>(glfwGetKey(window, static_cast<int>(key)));
		return key_state;
	}

	eKeyState InputPollGLFW::Mouse(eMouseCode button) const
	{
		GLFWwindow* window = window_->GetGLFWWindow();
		eKeyState button_state = static_cast<eKeyState>(glfwGetMouseButton(window, static_cast<int>(button)));
		return button_state;
	}

	float InputPollGLFW::MouseVerticalScroll() const
	{
		return 0.0f;
	}

	std::pair<float, float> InputPollGLFW::MousePos() const
	{
		GLFWwindow* window = window_->GetGLFWWindow();
		TY_CORE_ASSERT(window, "GLFWwindow is Null. Initialize windowing before calling input polling service provider.");

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return std::pair<float, float>(static_cast<float>(xpos), static_cast<float>(ypos));
	}
}