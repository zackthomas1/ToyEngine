#pragma once
#include "ToyEngine/events/observer.h"
#include "ToyEngine/services/time_step.h"
// GLFW
#include <GLFW/glfw3.h>
#include "imgui.h"

namespace ToyEngine
{
	class Layer :public Observer
	{
	public:
		virtual ~Layer() {}
		virtual void OnAttach() {};
		virtual void OnDetatch() {};
		/// <summary>
		/// Called on every frame from Application::Run
		/// Simulates one frame of the object’s behavior. 
		/// Each frame, the engine updates every layer in m_layerStack
		/// </summary>
		/// <param name="time_delta"></param>
		virtual void Update(TimeStep *time_step) {};
		/// <summary>
		/// Called in Application::Run after scene draw call.
		/// GUI defined in client application project. 
		/// ToyEngine maintains responsible for set up of dearimgui and lifetimes of layers.
		/// </summary>
		virtual void OnImGuiRender() {};
		/// <summary>
		/// Defines what events the layer consumes and how it responds.
		/// </summary>
		/// <param name="e"></param>
		virtual void OnEvent(Event& e) {};
	protected:
		Layer() {}
	};

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() {}
		virtual void OnAttach() override;
		virtual void OnDetatch() override;
		void BeginDraw();
		void EndDraw();
		virtual void OnEvent(Event& e) override;
	};

	class SceneLayer : public Layer
	{
	public:
		SceneLayer();
		~SceneLayer() {}

		virtual void OnAttach() override;
		virtual void OnDetatch() override;
		virtual void Update(TimeStep* time_step) override;
		virtual void OnEvent(Event& e) override;

	};
}