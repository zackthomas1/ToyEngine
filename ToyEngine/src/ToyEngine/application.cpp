#include "pch.h"
#include "ToyEngine/application.h"
#include "ToyEngine/renderer/renderer.h"

namespace ToyEngine
{
  Application* Application::s_instance = nullptr;

  Application::Application() : isRunning_(true)
  {
    TY_CORE_ASSERT(!s_instance, "Application already exist!")
    s_instance = this;

    // Initialize services
#ifdef TY_PLATFORM_WINDOWS
    // window
    services_.Register<Window, WindowsWindow>();
    Window& window = services_.Get<Window>();
    window.SetCommandCallbackFn(TY_BINDFN(Application::OnEvent));

    // time step
    services_.Register<TimeStep, TimeStepGLFW>();

    // input poll
    services_.Register<InputPoll, InputPollGLFW>(window);
#else
    services_.Register<Window, NullWindow>();
    services_.Register<TimeStep, NullTimeStep>();
    services_.Register<InputPoll, NullInputPoll>();
#endif TY_PLATFORM_WINDOWS

    // Initalize imgui layer
    imGuiLayer_ = new ImGuiLayer(services_.Get<Window>());
    layerStack_.PushLayer(imGuiLayer_);

    // Initialize renderer
    Renderer::Init();
  }

  void Application::Run()
  {
    // Initialize time step service. Starts the timer.
    TimeStep& time_step = services_.Get<TimeStep>();
    time_step.Init();

    // Get the window service for updating the window each frame.
    Window& window = services_.Get<Window>();

    while (isRunning_)
    {
      // Update variable time step
      time_step.Update();

      // Advance the game simulation one step (update)
      // Update layers
      for (Layer *layer : layerStack_)
        layer->Update(time_step.GetTimeDelta());

      // Draw GUI
      imGuiLayer_->BeginDraw();
      for(Layer *layer: layerStack_)
        layer->OnImGuiRender();
      imGuiLayer_->EndDraw();

      // Update the window (swap buffers, poll events)
      window.OnUpdate();
    }
  }

  void Application::OnEvent(Event& e)
  {
    EventDispatcher dispatcher(e); 
    dispatcher.Dispatch<EventApplicationClose>(TY_BINDFN(Application::OnClose));
    dispatcher.Dispatch<EventWindowResize>(TY_BINDFN(Application::OnResize));

    // Events are propagated from the topmost (foreground) layer to the bottom (background) layer.
    // This allows layers in the foreground to handle or consume events before they reach background layers.
    for (auto it = layerStack_.end(); it != layerStack_.begin(); ) {
      if (e.GetEventHandled()) { break; }
      (*--it)->OnEvent(e);
    }
  }
  
  void Application::PushLayer(Layer *layer)
  {
    layerStack_.PushLayer(layer);
  }

  void Application::PushOverlay(Layer *layer)
  {
    layerStack_.PushOverlay(layer);
  }

  bool Application::OnClose(EventApplicationClose& e)
  {
    isRunning_ = false;
    return !isRunning_;
  }

  bool Application::OnResize(EventWindowResize& e)
  {
    services_.Get<Window>().SetWindowSize(e.GetWidth(), e.GetHeight());
    return true;
  }
}