#include "pch.h"
#include "ToyEngine/services/window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ToyEngine
{
  WindowsWindow::WindowsWindow(const WindowProps& props)
    : Window(props)
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

  void WindowsWindow::SetCursorPos(float xpos, float ypos)
  {	
    if (window_)
      glfwSetCursorPos(window_, static_cast<double>(xpos), static_cast<double>(ypos));
    data_.x_mouse_pos = xpos;
    data_.y_mouse_pos = ypos;
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

    // set the user pointer to the window data structure so that it can be accessed in the callback functions.
    glfwSetWindowUserPointer(window_, &data_);

    // Uncomment to capture mouse in window and disable mouse visiability  
    //glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  void WindowsWindow::SetCallbackFns()
  {
    // Set GLFW callback functions for frame buffer size event which is triggered when the window is resized. 
    // The callback function updates the viewport and triggers a window resize event.
    glfwSetFramebufferSizeCallback(window_, [](GLFWwindow* window, int width, int height) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      
      // Make sure the viewport matches the new window dimensions.
      TY_CORE_TRACE("Update Framebuffer size: width - {} height - {}", width, height);
      glViewport(0, 0, width, height);
      
      // Create a window resize event and invoke the event callback function to notify the application of the change in window size.
      EventWindowResize window_resize(width, height);
      data.event_callback(window_resize);
    });

    // Set GLFW callback function for window close event which is triggered when the user attempts to close the window.
    glfwSetWindowCloseCallback(window_, [](GLFWwindow* window) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

      EventApplicationClose app_close = EventApplicationClose();
      data.event_callback(app_close);
    });

    // Set GLFW callback function for key input events which is triggered when a key is pressed, released, or held down.
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
    
    // Set GLFW callback function for scroll events which is triggered when the user scrolls the mouse wheel.
    glfwSetScrollCallback(window_, [](GLFWwindow* window, double x_offset, double y_offset) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      
      EventVerticalScroll scroll(static_cast<float>(y_offset));
      data.event_callback(scroll);
    });

    // Set GLFW callback function for cursor position events which is triggered when the user moves the mouse cursor within the window.
    glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double xpos, double ypos) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
      
      float x_current_pos = static_cast<float>(xpos);
      float y_current_pos = static_cast<float>(ypos);

      float x_ndc_coord_prev = (2.0f * (data.x_mouse_pos / data.width)) - 1.0f;
      float y_ndc_coord_prev = (2.0f * (data.y_mouse_pos / data.height)) - 1.0f;
      if (glm::abs(x_ndc_coord_prev) > 1.0f || glm::abs(y_ndc_coord_prev) > 1.0f){
        TY_CORE_WARN("Invalid NDC ({:.3f},{:.3f}) ignored. Out of expected range [-1,1]", x_ndc_coord_prev, y_ndc_coord_prev);
        return;
      }
      
      float x_ndc_coord = (2.0f * (x_current_pos / data.width)) - 1.0f;
      float y_ndc_coord = (2.0f * (y_current_pos / data.height)) - 1.0f;
      if (glm::abs(x_ndc_coord) > 1.0f || glm::abs(y_ndc_coord) > 1.0f) {
        TY_CORE_WARN("Invalid NDC ({:.3f},{:.3f}) ignored. Out of expected range [-1,1]", x_ndc_coord, y_ndc_coord);
        return;
      }
      
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

      bool is_released = static_cast<eKeyState>(glfwGetMouseButton(window, static_cast<int>(eMouseCode::kMouseMiddle))) == eKeyState::kRelease;
      
      EventCursorPos cursor_pos(x_offset, y_offset, x_ndc_coord_prev, y_ndc_coord_prev, x_ndc_coord, y_ndc_coord, is_released);
      data.event_callback(cursor_pos);
    });
  }

  void WindowsWindow::Shutdown()
  {
    TY_CORE_INFO("Window shutdown");
    glfwTerminate();
  }
}