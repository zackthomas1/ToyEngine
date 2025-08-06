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
		window_ = Window::Create();
		window_->SetCommandCallbackFn(TY_BINDFN(Application::OnEvent));

		// Initialize time step and input polling services
		TY_CORE_INFO("Initialize time step service");
		Locator::TimeStepService().Init();
		TY_CORE_INFO("Initialize input poll service");
		Locator::InputPollService().Init();

		// Initalize imgui layer
		imGuiLayer_ = new ImGuiLayer();
		layerStack_.PushLayer(imGuiLayer_);

		// initialize renderer
		Renderer::Init();
	}

	Application::~Application()
	{
		Locator::DestroyServiceProviders();
	}

	void Application::Run()
	{
		while (isRunning_)
		{
			// Update variable time step
			Locator::TimeStepService().Update();

			// Advance the game simulation one step (update)
			// Update layers
			for (Layer *layer : layerStack_)
			{
				layer->Update(Locator::TimeStepService());
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
		window_->SetWindowSize(e.GetWidth(), e.GetHeight());
		return true;
	}
}