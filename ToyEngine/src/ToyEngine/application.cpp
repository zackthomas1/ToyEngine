#include "pch.h"
#include "ToyEngine/application.h"
#include "ToyEngine/events/event_handler.h"
#include "ToyEngine/layers/layer.h"
#include "ToyEngine/layers/layer_stack.h"
#include "ToyEngine/services/time_step_glfw.h"
#include "ToyEngine/services/input_poll_glfw.h"
#include "ToyEngine/services/locator.h"

namespace ToyEngine
{
	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		TY_CORE_ASSERT(!s_instance, "Application already exist!")
		s_instance = this;

		// Initialize window
		window_ = std::unique_ptr<WindowsWindow>(WindowsWindow::Create());
		window_->SetCommandCallbackFn(TY_BINDFN(Application::OnEvent));

		// Initialize time step service
		Locator::SetTimeStepProvider(new TimeStepGLFW());
		
		// Initialize input polling service
		Locator::SetInputPollProvider(new InputPollGLFW());

		// Initalize imgui layer
		imGuiLayer_ = new ImGuiLayer();
		layerStack_.PushLayer(imGuiLayer_);

		// Create a Scene
		// TODO: Move scene creation out of the application class
		scene_ = std::make_shared<Scene>();

		// initialize renderer
		renderer_ = std::unique_ptr<Renderer>(Renderer::Create());
	}

	Application::~Application()
	{
		Locator::DeleteTimeStepProvider();
		Locator::DeleteInputPollProvider();
		
		// Note: window_, scene_, and render_ are smart pointers that manage the memory they point to.
		// There is no need to manually deallocate memory for them.
	}

	void Application::Run()
	{
		while (isRunning_)
		{
			// Update variable time step
			Locator::TimeStepService()->Update();

			// Advance the game simulation one step (update)
			TimeStep *time_step = Locator::TimeStepService();
			
			// Update layers
			for (Layer *layer : layerStack_)
			{
				layer->Update(time_step);
			}
			scene_->Update(time_step);

			// Draw the game
			renderer_->DrawScene(scene_);

			// Draw GUI
			imGuiLayer_->BeginDraw();
			for(Layer *layer: layerStack_)
			{
				layer->OnImGuiRender();
			}
			imGuiLayer_->EndDraw();

			window_->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		if (EventApplicationClose* event = dynamic_cast<EventApplicationClose*>(&e)) {
			e.SetEventHandled(OnClose());
		}

		// Event handling starts at the top of the layer stack
		// Layer in the foreground attempt handle events before background layers	
		for (auto it = layerStack_.end(); it != layerStack_.end(); ) {
			(*--it)->OnEvent(e);
			if(e.GetEventHandled()) { break; }
		}
	}
	void Application::PushLayer(Layer *layer)
	{
		layerStack_.PushLayer(layer);
	}

	void Application::PushOverlay(Layer *layer)
	{
		TY_CORE_WARN("TODO: Implement Application::PushOverlay");
	}

	bool Application::OnClose() {
		isRunning_ = false;
		return !isRunning_;
	}

}