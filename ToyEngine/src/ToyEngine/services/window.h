#pragma once
#include "ToyEngine/event.h"

/// Forward declaration of GLFWwindow to avoid including GLFW headers in this header file.
struct GLFWwindow;

namespace ToyEngine {
  /// @brief Structure that holds properties for creating a window.
  struct WindowProps
  {
    /* The title of the window. */
    std::string title;

    /* Flag indicating whether the mouse is active. */
    bool is_mouse_active;

    /* The width and height of the window in pixels. */
    unsigned int width, height;

    /* The current x and y position of the mouse. Updated when the mouse moves. */
    float x_mouse_pos, y_mouse_pos;

    /// @brief Constructs a WindowProps object with the specified parameters. If no parameters are provided, default values are used.
    /// @param title: The title of the window. Default is "Editor".
    /// @param is_mouse_active: Flag indicating whether the mouse is active. Default is false.
    /// @param width: The width of the window in pixels. Default is TY_DEFAULT_WINDOW_WIDTH.
    /// @param height: The height of the window in pixels. Default is TY_DEFAULT_WINDOW_HEIGHT.
    /// @param x_mouse_pos: The initial x position of the mouse. Default is the center of the window.
    /// @param y_mouse_pos: The initial y position of the mouse. Default is the center of the window.
    WindowProps(const std::string& title = "Editor",
      bool is_mouse_active = false,
      unsigned int width = TY_DEFAULT_WINDOW_WIDTH,
      unsigned int height = TY_DEFAULT_WINDOW_HEIGHT,
      float x_mouse_pos = TY_DEFAULT_WINDOW_WIDTH / 2.0f,
      float y_mouse_pos = TY_DEFAULT_WINDOW_HEIGHT / 2.0f)
        : title(title), is_mouse_active(is_mouse_active),
        width(width), height(height),
        x_mouse_pos(x_mouse_pos), y_mouse_pos(y_mouse_pos)
    {}
  };

  class Window
  {
  public:
    /// @brief Type alias for the event callback function.
    /// The callback takes a reference to an Event object and returns void.
    using EventCallbackFn = std::function<void(Event&)>;

    /// @brief Virtual destructor for the Window class.
    virtual ~Window() = default;

    /// @brief Creates a new window instance with the specified properties.
    /// This is a static factory method that returns a unique pointer to a Window object.
    static Scope<Window> Create(const WindowProps& props = WindowProps());

    /// @brief Updates the window state, processes events, and renders the contents.
    virtual void OnUpdate() = 0;
    
    /// @brief Retrieves a pointer to the native window object. 
    /// This method must be implemented by derived classes to return the platform-specific window handle.
    virtual void* GetNativeWindow() const = 0;
    
    /// @brief sets the callback function that will be invoked when an event occurs in the window.
    /// the callback receives a reference to an event object, allowing custom event handling logic.
    /// this method must be implemented by derived classes to connect the window's event system
    /// with the application's event processing code.
    /// @param callback: The callback function to be invoked when an event occurs.
    virtual void SetCommandCallbackFn(const EventCallbackFn& callback) = 0;
    
    /// @brief Sets the cursor position within the window.
    /// This method must be implemented by derived classes to update the cursor position.
    virtual void SetCursorPos(float xpos, float ypos) = 0;

    /// @brief Retrieves the current width of the window in pixels.
    int GetWidth() const { return data_.width; }

    /// @brief Retrieves the current height of the window in pixels.
    int GetHeight() const { return data_.height; }

    /// @brief Retrieves the current aspect ratio of the window (width / height).
    float GetAspectRatio() const { return static_cast<float>(data_.width) / static_cast<float>(data_.height); }

    /// @brief Sets the size of the window in pixels.
    void SetWindowSize(unsigned int width, unsigned int height) { data_.width = width; data_.height = height; }
  protected:
    /// @brief Constructs a Window object with the specified properties.
    /// The Constructor is protected to prevent direct instantiation of the Window class, as it is intended to be an abstract base class.
    Window() = default;

    /// @brief Constructs a Window object with the specified properties.
    /// The Constructor is protected to prevent direct instantiation of the Window class, as it is intended to be an abstract base class.
    /// @param props: The properties to initialize the window with.
    Window(const WindowProps& props);

    /// @brief Structure that holds the internal data for the window, including title, size, event callback, and mouse state.
    struct WindowData
    {
      /* The title of the window. */
      std::string title;
      /* The width of the window in pixels. */
      unsigned int width;
      /* The height of the window in pixels. */
      unsigned int height;
      /* The callback function to handle window events. */
      EventCallbackFn event_callback;
      /* Indicates whether the mouse is currently active within the window. */
      bool is_mouse_active;
      /* The current x-coordinate of the mouse cursor within the window. */
      float x_mouse_pos;
      /* The current y-coordinate of the mouse cursor within the window. */
      float y_mouse_pos;
    };
    /* The internal data structure that holds the window's properties and state. */
    WindowData data_;
  };

  /// @brief A NullWindow class that implements the Window interface but does not perform any actual window operations.
  class NullWindow : public Window
  {
    virtual void OnUpdate() override {}
    virtual void* GetNativeWindow() const override { return nullptr; }
    virtual void SetCommandCallbackFn(const EventCallbackFn& callback) override { data_.event_callback = callback; }
    virtual void SetCursorPos(float xpos, float ypos) override {}
  };

  /// @brief A Windows-specific implementation of the Window interface using GLFW for window management.
  class WindowsWindow : public Window
  {
  public:
    /// @brief Constructs a WindowsWindow object with the specified properties.
    /// @param props: The properties to initialize the window with.
    WindowsWindow(const WindowProps& props = WindowProps());

    /// @brief Destructor for the WindowsWindow class. Cleans up resources and shuts down the window.
    ~WindowsWindow();

    /// @brief Updates the window state, processes events, and renders the contents.
    virtual void OnUpdate() override;

    /// @brief Retrieves a pointer to the native window object.
    virtual void* GetNativeWindow() const override { return window_; }

    /// @brief Sets the callback function that will be invoked when an event occurs in the window.
    /// @param callback: The callback function to handle window events. Defined in the application class and passed to the window service.
    virtual void SetCommandCallbackFn(const EventCallbackFn& callback) override { data_.event_callback = callback; }
    
    /// @brief Sets the cursor position within the window.
    /// @param xpos: The x-coordinate of the cursor.
    /// @param ypos: The y-coordinate of the cursor.
    virtual void SetCursorPos(float xpos, float ypos) override;

  private:
    /// @brief Initializes the GLFW window, sets up OpenGL context, and configures callbacks.
    void Init();

    /// @brief Sets up the GLFW callback functions for handling window events such as resizing, key input, mouse movement, and scrolling.
    void SetCallbackFns();

    /// @brief Shuts down the GLFW window and cleans up resources.
    void Shutdown();

  private:
    /* Pointer to the GLFW window object. This is used for managing the window and its events. */
    GLFWwindow* window_;
  };
}