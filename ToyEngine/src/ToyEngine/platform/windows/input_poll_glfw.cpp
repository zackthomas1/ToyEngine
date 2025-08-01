#include "pch.h"
#include "ToyEngine/application.h"
#include "ToyEngine/services/input_poll.h"
#include "ToyEngine/platform/windows/windows_window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ToyEngine {
    void InputPollGLFW::Init() 
    {
        TY_CORE_INFO("GLFW input poll provider");
    }

    eKeyState InputPollGLFW::Key(eKeyCode key)
    {
        GLFWwindow* window = dynamic_cast<WindowsWindow*>(&Application::AccessWindow())->GetGLFWWindow();
        TY_CORE_ASSERT(window, "GLFWwindow is Null. Initialize windowing before calling input polling service provider.");
        eKeyState key_state = static_cast<eKeyState>(glfwGetKey(window, static_cast<int>(key)));
        return key_state;
    }

    eKeyState InputPollGLFW::Mouse(eMouseCode button)
    {
        GLFWwindow* window = dynamic_cast<WindowsWindow*>(&Application::AccessWindow())->GetGLFWWindow();
        eKeyState botton_state = static_cast<eKeyState>(glfwGetMouseButton(window, static_cast<int>(button)));
        return botton_state;
    }

    float InputPollGLFW::MouseVerticalScroll()
    {
        return 0.0f;
    }

    std::pair<float, float> InputPollGLFW::MousePos()
    {
        GLFWwindow* window = dynamic_cast<WindowsWindow*>(&Application::AccessWindow())->GetGLFWWindow();
        TY_CORE_ASSERT(window, "GLFWwindow is Null. Initialize windowing before calling input polling service provider.");

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return std::pair<float, float>(static_cast<float>(xpos), static_cast<float>(ypos));
    }
}