#pragma once
#include "core.h"
#include "ToyEngine/event.h"
#include "ToyEngine/layers/layer_stack.h"
#include "ToyEngine/services/service_container.h"

namespace ToyEngine
{
  class Application
  {
  public:
    /// @brief Constructs the Application instance and initializes core services, layers, and renderer.
    Application();

    /// @brief Virtual destructor for the Application class.
    virtual ~Application() = default;

    /// @brief Deleted copy constructor to prevent copying of the Application instance.
    /// Application is intended to be a singleton and should not be copied.
    Application(const Application&) = delete;

    /// @brief Deleted copy assignment operator to prevent copying of the Application instance.
    /// Application is intended to be a singleton and should not be copied.
    Application& operator=(const Application&) = delete;
    
    /// @brief Deleted move constructor to prevent moving of the Application instance.
    /// Application is intended to be a singleton and should not be moved.
    Application(Application&&) = delete;

    /// @brief Deleted move assignment operator to prevent moving of the Application instance.
    /// Application is intended to be a singleton and should not be moved.
    Application& operator=(Application&&) = delete;
    
    /// @brief Starts and manages the main application loop.
    /// Continuously updates the window, processes events, and updates all layers
    /// until the application is closed.
    void Run();

    /// @brief Handles incoming events and dispatches them to the appropriate layers.
    /// This function is called whenever an event occurs (e.g., input, window events).
    /// It propagates the event through the layer stack in reverse order (from topmost to bottom),
    /// allowing each layer to handle or consume the event as needed.
    /// @param e: Reference to the event object to be processed.
    void OnEvent(Event& e);

    /// @brief Adds a new layer to the application. Layers are used for core engine logic and are updated and rendered in order.
    /// The layer is inserted below overlays and above previously added layers.
    /// @param layer: Pointer to the Layer to be added.
    void PushLayer(Layer *layer);

    /// @brief Adds a new overlay to the application. Overlays are rendered and updated after all regular layers.
    /// Typically used for UI or debug panels.
    /// @param layer: Pointer to the Layer to be added as an overlay.
    void PushOverlay(Layer *layer);

    // Accessors return const references to prevent modification
    //inline static Application& Get() { return *s_instance; }

  protected:
    /// @brief Gets a reference to the ServiceContainer instance used by the application.
    /// The ServiceContainer manages core services such as windowing, time step, and input polling.
    /// @return Reference to the ServiceContainer instance.
    inline static ServiceContainer& GetServices() { return Application::s_instance->services_; }
  private:
    /// @brief Event handler for application close events. Sets the running flag to false to exit the main loop.
    /// @param e: Reference to the EventApplicationClose object.
    /// @return True if the event was handled, false otherwise.
    bool OnClose(EventApplicationClose& e);

    /// @brief Event handler for window resize events. Updates the renderer and any relevant layers to accommodate the new window size.
    /// @param e: Reference to the EventWindowResize object.
    /// @return True if the event was handled, false otherwise.
    bool OnResize(EventWindowResize& e);

    /* Service container instance used by the application. Manages core services such as windowing, time step, and input polling. */
    ServiceContainer services_;

    /* imGuiLayer is owned by the layerStack_. Deleted by layerStack_ destructor */
    ImGuiLayer *imGuiLayer_;

    /* Layer stack used to manage layers and overlays. */
    LayerStack layerStack_;

    /* Flag indicating whether the application is running. Constructor initializes this to true. */
    bool isRunning_;

    /* Singleton instance of the application. Only one instance exists. */
    static Application *s_instance;
  };

  /// @brief Factory function to create an instance of the Application class.
  /// This function should be implemented by the client application to return a new instance of their specific Application subclass.
  /// @return Pointer to the newly created Application instance.
  Application* CreateApplication();
}