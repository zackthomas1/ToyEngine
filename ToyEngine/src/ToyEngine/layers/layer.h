#pragma once
#include "ToyEngine/event.h"

// imgui
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"

namespace ToyEngine
{
  // Forward declaration of Window class to avoid circular dependency issues.
  class Window;

  /// @brief The Layer class represents a layer in the application, which can handle events, update logic, and render GUI elements.
  class Layer
  {
  public:
    /// @brief Virtual destructor for the Layer class. Ensures proper cleanup of derived classes.
    virtual ~Layer() = default;
    
    /// @brief Called when the layer is added to the layer stack.
    /// This method is called by the LayerStack::Pushlayer and LayerStack::PushOverlay.
    virtual void OnAttach() {};
    
    /// @brief Called when the layer is removed from the layer stack.
    virtual void OnDetatch() {};
    
    /// @brief Called on every frame from Application::Run
    /// Simulates one frame of the object behavior. 
    /// Each frame, the engine updates every layer in m_layerStack
    /// @param time_delta: The time elapsed since the last frame, in seconds.
    virtual void Update(float time_delta) {};
    
    /// @brief Called in Application::Run after scene draw call.
    /// GUI defined in client application project. 
    /// ToyEngine maintains responsible for set up of dearimgui and lifetimes of layers.
    virtual void OnImGuiRender() {};
    
    /// @brief Defines what events the layer consumes and how it responds.
    /// @param e: The event to handle.
    virtual void OnEvent(Event& e) {};
  protected:
    /// @brief Default constructor for the Layer class. Protected to prevent direct instantiation. 
    Layer() = default;
  };

  /// @brief The IImGuiImpl class defines an interface for ImGui implementation.
  /// This interface allows for different backend implementations of ImGui, such as GLFW or other windowing systems.
  class IImGuiImpl
  {
  public:
    /// @brief Default constructor for the IImGuiImpl class.
    IImGuiImpl() = default;

    /// @brief Virtual destructor for the IImGuiImpl class. Ensures proper cleanup of derived classes.
    virtual ~IImGuiImpl() = default;

    /// @brief Initializes the ImGui implementation. This method must be called before using ImGui.
    virtual void Init() = 0;

    /// @brief Prepares a new frame for ImGui rendering.
    virtual void NewFrame() = 0;

    /// @brief Finalizes the current frame and renders ImGui draw data.
    virtual void EndFrame() = 0;

    /// @brief Shuts down the ImGui implementation and releases any allocated resources.
    virtual void Shutdown() = 0;

    /// @brief Creates an instance of the ImGui implementation for the specified window.
    /// @param window: The window for which to create the ImGui implementation.
    /// @return A scoped pointer to the created ImGui implementation.
    static Scope<IImGuiImpl> Create(Window& window);
  };

  /// @brief The ImGuiLayer class is a layer that integrates Dear ImGui into the application.
  class ImGuiLayer : public Layer
  {
  public:
    /// @brief Constructs an ImGuiLayer instance and initializes the ImGui implementation.
    ImGuiLayer(Window& window, bool blocks_event = true);

    /// @brief Called when the layer is added to the layer stack.
    /// This method is called by the LayerStack::Pushlayer and LayerStack::PushOverlay.
    virtual void OnAttach() override;

    /// @brief Called when the layer is removed from the layer stack.
    /// This method is called by the LayerStack::PopLayer and LayerStack::PopOverlay.
    virtual void OnDetatch() override;

    /// @brief Defines what events the layer consumes and how it responds.
    /// @param e: The event to handle.
    virtual void OnEvent(Event& e) override;
   
    /// @brief Prepares a new frame for ImGui rendering.
    void BeginDraw();

    /// @brief Finalizes the current frame and renders ImGui draw data.
    void EndDraw();

    /// @brief Sets whether the ImGuiLayer should block events from reaching other layers.
    /// @param blocking: If true, the ImGuiLayer will block events from reaching other layers.
    void BlockEvents(bool blocking) { blocks_event_ = blocking; }
  private:
    /* Whether the ImGuiLayer should block events from reaching other layers. */
    bool blocks_event_;

    /* Pointer to the ImGui implementation used by this layer. */
    Scope<IImGuiImpl> impl_;
  };

  /// @brief The ImGuiImplGLFW class is a concrete implementation of the IImGuiImpl interface for GLFW.
  class ImGuiImplGLFW : public IImGuiImpl
  {
  public:
    /// @brief Constructs an ImGuiImplGLFW instance for the specified GLFW window.
    /// @param window: Pointer to the GLFW window associated with this ImGui implementation.
    ImGuiImplGLFW(GLFWwindow* window) : window_(window) {}

    /// @brief Destructor for the ImGuiImplGLFW class. Ensures proper cleanup of resources.
    ~ImGuiImplGLFW() = default;

    /// @brief Initializes the ImGui implementation. This method must be called before using ImGui.
    virtual void Init() override;

    /// @brief Prepares a new frame for ImGui rendering.
    virtual void NewFrame() override;
    /// @brief Finalizes the current frame and renders ImGui draw data.
    virtual void EndFrame() override;

    /// @brief Shuts down the ImGui implementation and releases any allocated resources.
    virtual void Shutdown() override;
  private:
    /* A pointer to underlying GLFW window object associated with this ImGui implementation. */
    GLFWwindow* window_;
  };
}