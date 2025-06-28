#include "pch.h"
#include "ToyEngine/application.h"
#include "ToyEngine/events/event_handler.h"
#include "ToyEngine/layers/layer.h"
#include "ToyEngine/layers/layer_stack.h"
#include "ToyEngine/services/time_step_glfw.h"
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

		// Initialize time step
		Locator::SetTimeStepProvider(new TimeStepGLFW());

		//
		imGuiLayer_ = new ImGuiLayer();
		layerStack_.PushLayer(imGuiLayer_);

		// Create a Scene
		// TODO: Move scene creation on of the application class
		scene_ = std::make_shared<Scene>();

		// initialize renderer
		renderer_ = std::unique_ptr<Renderer>(Renderer::Create());
		renderer_->SetRenderCamera(scene_->GetCameras()[0]);
	}

	Application::~Application()
	{
		Locator::DeleteTimeStepProvider();
		
		// Note: window_, scene_, and render_ are smart pointers that manage the memory they point to.
		// There is no need to manually deallocate memory for them.
	}

	void Application::Run()
	{
		while (!window_->ShouldClose())
		{
			// Update variable time step
			Locator::TimeStepService()->Update();

			// Handle any user input since the last call
			window_->ProcessInput();

			// Advance the game simulation one step (update)
			float time_delta = Locator::TimeStepService()->GetTimeStep();
			
			// Update
			for (Layer *layer : layerStack_)
			{
				layer->Update(time_delta);
			}
			scene_->Update(time_delta);

			// Draw the game
			renderer_->DrawScene(scene_);

			// Draw GUI
			imGuiLayer_->BeginDraw();
			for(Layer *layer: layerStack_)
			{
				layer->OnImGuiRender();
			}
			imGuiLayer_->EndDraw();

			window_->SwapBuffers();
			window_->PollEvents();
		}
	}

	void Application::OnEvent(Event& e)
	{
		for (Layer* layer : layerStack_) {
			layer->OnEvent(e);
		}
		s_instance->renderer_->OnEvent(e);
	}
	void Application::PushLayer(Layer *layer)
	{
		layerStack_.PushLayer(layer);
	}

	void Application::PushOverlay(Layer *layer)
	{
		TY_CORE_WARN("TODO: Implement Application::PushOverlay");
	}
}