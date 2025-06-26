#pragma once
#include "ToyEngine/events/observer.h"

// GLFW
#include <GLFW/glfw3.h>

#include "imgui.h"

namespace ToyEngine
{
	class Layer :public Observer
	{
	public:
		virtual ~Layer() {}
		virtual void Update(float time_delta) = 0;
		virtual void OnEvent(Event& e) = 0;
	protected:
		Layer() {}
	};

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() {}
		virtual void Update(float time_delta) override; 
		virtual void OnEvent(Event& e) override;
	protected:
		ImGuiIO& io_;

		/// <summary>
		/// Called by Lauer::Update(float time_delta) method. Defined in child class that inherits from ImGuiLayer.
		/// This allows the user to define the GUI in the their own application project, but with ToyEngine 
		/// still responsible for the set up of dearimgui and the lifetimes of the gui layers.
		/// </summary>
		virtual void CreateWindows() = 0;
	};
}