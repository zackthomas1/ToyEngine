#include "pch.h"
#include "ToyEngine/services/input_poll.h"
#include "ToyEngine/services/window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ToyEngine {
	InputPollGLFW::InputPollGLFW(Window& window)
	{
		window_ = static_cast<GLFWwindow*>(window.GetNativeWindow());
		TY_CORE_ASSERT(window_ != nullptr, "GLFWwindow pointer is null or invalid");
	}

	void InputPollGLFW::Init()
	{
		TY_CORE_INFO("GLFW input poll provider");
	}

	eKeyState InputPollGLFW::Key(eKeyCode key) const
	{
		eKeyState key_state = static_cast<eKeyState>(glfwGetKey(window_, static_cast<int>(key)));
		return key_state;
	}

	eKeyState InputPollGLFW::Mouse(eMouseCode button) const
	{
		eKeyState button_state = static_cast<eKeyState>(glfwGetMouseButton(window_, static_cast<int>(button)));
		return button_state;
	}

	float InputPollGLFW::MouseVerticalScroll() const
	{
		return 0.0f;
	}

	std::pair<float, float> InputPollGLFW::MousePos() const
	{
		double xpos, ypos;
		glfwGetCursorPos(window_, &xpos, &ypos);
		return std::pair<float, float>(static_cast<float>(xpos), static_cast<float>(ypos));
	}
}