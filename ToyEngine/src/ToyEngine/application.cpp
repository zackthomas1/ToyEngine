#include "pch.h"
#include "ToyEngine/application.h"
#include "ToyEngine/renderer/renderer.h"
#include "ToyEngine/layers/layer.h"
#include "ToyEngine/services/locator.h"

namespace ToyEngine
{
	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		TY_CORE_ASSERT(!s_instance, "Application already exist!")
		s_instance = this;

		// Initialize window
		window_ = Scope<Window>(Window::Create());
		window_->SetCommandCallbackFn(TY_BINDFN(Application::OnEvent));

		// Initialize time step and input polling services
		TY_CORE_INFO("Initialize time step service");
		Locator::TimeStepService()->Init();
		TY_CORE_INFO("Initialize input poll service");
		Locator::InputPollService()->Init();

		// Initalize imgui layer
		imGuiLayer_ = new ImGuiLayer();
		layerStack_.PushLayer(imGuiLayer_);

		// initialize renderer
		Renderer::Init();
	}

	Application::~Application()
	{
		Locator::DestoryServiceProviders();
		
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
			float delta_time = Locator::TimeStepService()->GetTimeStep();
			// Update layers
			for (Layer *layer : layerStack_)
			{
				layer->Update(delta_time);
			}

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
		for (auto it = layerStack_.end(); it != layerStack_.begin(); ) {
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