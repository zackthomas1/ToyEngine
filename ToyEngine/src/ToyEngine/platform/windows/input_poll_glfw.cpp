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

    eKeyState ToyEngine::InputPollGLFW::Key(eKeyCode key)
    {
        GLFWwindow* window = dynamic_cast<WindowsWindow*>(&Application::Get().GetWindow())->GetGLFWWindow();
        TY_CORE_ASSERT(window, "GLFWwindow is Null. Initialize windowing before calling input polling service provider.");
        eKeyState key_state = static_cast<eKeyState>(glfwGetKey(window, static_cast<int>(key)));
        return key_state;
    }

    float ToyEngine::InputPollGLFW::MouseVerticalScroll()
    {
        return 0.0f;
    }

    std::pair<float, float> ToyEngine::InputPollGLFW::MousePos()
    {
        GLFWwindow* window = dynamic_cast<WindowsWindow*>(&Application::Get().GetWindow())->GetGLFWWindow();
        TY_CORE_ASSERT(window, "GLFWwindow is Null. Initialize windowing before calling input polling service provider.");

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return std::pair<float, float>(static_cast<float>(xpos), static_cast<float>(ypos));
    }
}