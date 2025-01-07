#pragma once
#include "ToyEngine/events/observer.h"

// GLFW
#include <GLFW/glfw3.h>

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
		ImGuiLayer() {}
		~ImGuiLayer() {}
		virtual void Update(float time_delta) override; 
		virtual void OnEvent(Event& e) override;
	};
}