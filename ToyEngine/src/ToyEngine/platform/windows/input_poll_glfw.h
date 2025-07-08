#pragma once
#include "ToyEngine/services/input_poll.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ToyEngine {
    
    class InputPollGLFW : public InputPoll{
    public:
        InputPollGLFW();
        ~InputPollGLFW() {}

        virtual eKeyState Key(eKeyCode key) override;
        virtual float MouseVerticalScroll() override;
        virtual std::pair<float, float> MousePos() override;
    private:
        GLFWwindow *window_;
    };
}