#pragma once
#include "pch.h"

#include "ToyEngine/layers/layer.h"
#include "ToyEngine/renderer/model.h"
#include "ToyEngine/renderer/light.h"
#include "ToyEngine/renderer/camera/camera.h"
#include "ToyEngine/services/time_step.h"

namespace ToyEngine
{
	class SceneLayer : public Layer
	{
	public:
		SceneLayer();
		~SceneLayer();

		virtual void OnAttach() override;
		virtual void OnDetatch() override;
		virtual void Update(TimeStep* time_step) override;
		void OnDraw();
		virtual void OnEvent(Event& e) override;

	public:
		Camera* camera_;
		std::vector<std::shared_ptr<Model>> models_;
		//std::vector<Light> lights_; 
	};
}