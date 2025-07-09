#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ToyEngine/window.h"

namespace ToyEngine
{
    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProps& props);
        ~WindowsWindow();

        virtual void OnUpdate() override;

        virtual void SetCommandCallbackFn(const EventCallbackFn& callback) { data_.event_callback = callback; }
        GLFWwindow* GetGLFWWindow() const { return window_; } // Mark as const to prevent modification of the class or pointer

    private:
        void Init();
        void SetCallbackFns();
        void Shutdown();

    private: 
        GLFWwindow* window_; 
    };
}