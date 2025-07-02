#include "pch.h"
#include "input_poll_glfw.h"
#include "ToyEngine/application.h"
#include "ToyEngine/windows_window.h"

namespace ToyEngine {

    InputPollGLFW::InputPollGLFW()
    {
        TY_CORE_ASSERT(Application::Get().GetWindow().GetGLFWWindow(), "GLFWwindow is Null."
            "Initialize windowing before setting input polling service provider.");
        window_ = Application::Get().GetWindow().GetGLFWWindow();
    }

    KeyState ToyEngine::InputPollGLFW::Key(KeyCode key)
    {
        KeyState key_state = static_cast<KeyState>(glfwGetKey(window_, static_cast<int>(key)));
        return key_state;
    }

    float ToyEngine::InputPollGLFW::MouseVerticalScroll()
    {
        return 0.0f;
    }

    std::pair<float, float> ToyEngine::InputPollGLFW::MousePos()
    {
        double xpos, ypos;
        glfwGetCursorPos(window_, &xpos, &ypos);
        return std::pair<float, float>(static_cast<float>(xpos), static_cast<float>(ypos));
    }
}