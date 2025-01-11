#include "pch.h"
#include "ToyEngine/application.h"

#include "ToyEngine/events/event_handler.h"

#include "ToyEngine/layers/layer.h"
#include "ToyEngine/layers/layer_manager.h"

#include "ToyEngine/services/time_step_glfw.h"
#include "ToyEngine/services/locator.h"

#include "ToyEngine/renderer/gui_renderer.h"

namespace ToyEngine
{
	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		TY_CORE_ASSERT(!s_instance, "Application already exist!")
		s_instance = this;

		// Initialize window
		window_ = std::unique_ptr<WindowsWindow>(WindowsWindow::Create());
		window_->SetCommandCallbackFn(Application::EventHandler);

		// Initialize Gui
		GuiRenderer::Init(window_->GetGLFWWindow());
		
		// Initialize time step
		Locator::SetTimeStepProvider(new TimeStepGLFW());

		// Create a Scene
		// TODO: Move scene creation on of the application class
		scene_ = std::make_shared<Scene>();

		// initialize renderer
		renderer_ = std::unique_ptr<Renderer>(Renderer::Create());
		renderer_->SetRenderCamera(scene_->GetCameras()[0]);
	}

	Application::~Application()
	{
		GuiRenderer::Delete();
		Locator::DeleteTimeStepProvider();
		LayerManager::DeleteLayers();
		
		// Note: window_, scene_, and render_ are smart pointers that also manage the memory they point to. 
		// There is no need to manually deallocate memory for them.
	}

	void Application::Update(float time_delta)
	{
		LayerManager::UpdateLayers(time_delta);
		scene_->Update(time_delta);
	}

	void Application::Run()
	{
		while (!window_->ShouldClose())
		{
			// Update variable time step
			Locator::TimeStepService()->Update();

			// Handle any user input since the last call
			window_->ProcessInput();

			// Advance the game simulation one step
			Update(Locator::TimeStepService()->GetTimeStep());

			// Draw the game
			renderer_->DrawScene(scene_);
			GuiRenderer::DrawGui();

			window_->SwapBuffers();
			window_->PollEvents();
		}
	}

	void Application::EventHandler(Event& e)
	{
		LayerManager::OnEvent(e);
		s_instance->renderer_->OnEvent(e);
	}
}