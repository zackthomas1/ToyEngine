#pragma once
#include "ToyEngine/enum.h"

// Forward declaration of GLFWwindow to avoid including GLFW headers in this header file.
struct GLFWwindow;

namespace ToyEngine {
  /// @brief The abstract InputPoll class provides an interface for polling input states from the user.
  class InputPoll {
  public:
    /// @brief Virtual destructor for the InputPoll class.
    virtual ~InputPoll() = default;

    /// @brief Retrieves the current state of a specified keyboard key.
    /// @param key: The key code to query.
    /// @return The current state of the key (pressed, released, or held).
    virtual eKeyState Key(eKeyCode key)         const = 0;
    
    /// @brief Retrieves the current state of a specified mouse button.
    /// @param button: The mouse button code to query.
    /// @return The current state of the mouse button (pressed, released, or held).
    virtual eKeyState Mouse(eMouseCode button)  const = 0;

    /// @brief Retrieves the vertical scroll value of the mouse wheel.
    /// @return The vertical scroll value, where positive values indicate scrolling up and negative values indicate scrolling down.
    virtual float MouseVerticalScroll()         const = 0;

    /// @brief Retrieves the current position of the mouse cursor.
    /// @return A pair of floats representing the x and y coordinates of the mouse cursor.
    virtual std::pair<float, float> MousePos()  const = 0;
  protected:
    InputPoll() {}
  };

  class NullInputPoll : public InputPoll {
  public:
    NullInputPoll() = default;

    virtual eKeyState Key(eKeyCode key)        const override { return eKeyState::kRelease; }
    virtual eKeyState Mouse(eMouseCode button) const override { return eKeyState::kRelease; }
    virtual float MouseVerticalScroll()        const override { return 0.0f; }
    virtual std::pair<float, float> MousePos() const override { return std::pair<float, float>(0.0f, 0.0f); }
  };

  // Forward declaration of Window class to avoid circular dependency issues.
  class Window;

  class InputPollGLFW : public InputPoll {
  public:
    /// @brief Constructs an InputPollGLFW instance that polls input states from a GLFW window.
    /// @param window: Reference to the Window object from which to poll input states.
    InputPollGLFW(Window& window);

    /// @brief Destructor for the InputPollGLFW class.
    ~InputPollGLFW() = default;

    /// @brief Retrieves the current state of a specified keyboard key.
    /// @param key: The key code to query.
    /// @return The current state of the key (pressed, released, or held).
    virtual eKeyState Key(eKeyCode key)        const override;

    /// @brief Retrieves the current state of a specified mouse button.
    /// @param button: The mouse button code to query.
    /// @return The current state of the mouse button (pressed, released, or held).
    virtual eKeyState Mouse(eMouseCode button) const override;

    /// @brief Retrieves the vertical scroll value of the mouse wheel.
    /// @return The vertical scroll value, where positive values indicate scrolling up and negative values indicate scrolling down.
    virtual float MouseVerticalScroll()        const override;

    /// @brief Retrieves the current position of the mouse cursor.
    /// @return A pair of floats representing the x and y coordinates of the mouse cursor.
    virtual std::pair<float, float> MousePos() const override;
  private:
    /* Pointer to the underlying GLFW window associated with this InputPollGLFW instance. */
    GLFWwindow* window_ = nullptr;
  };
}