#pragma once
#include "pch.h"
#include "ToyEngine/events/observer.h"

#include "ToyEngine/renderer/model.h"
#include "ToyEngine/renderer/light.h"
#include "ToyEngine/renderer/camera/camera.h"

namespace ToyEngine
{
	class Scene : public Observer
	{
	public:
		Scene();
		~Scene();

		void Update(float time_delta);
		
		// Observer methods
		virtual void OnEvent(Event& event) override;

		// scene modifiers
		void AddModel(std::shared_ptr<Model> model);
		//void AddLight(Light light); 

		// getter/setter
		const std::vector<std::shared_ptr<Model>>& GetModels() const { return models_; }
		const std::vector<Camera*>& GetCameras() const { return cameras_; }

	private:
		std::vector<Camera*> cameras_;
		std::vector<std::shared_ptr<Model>> models_; 
		//std::vector<Light> lights_; 

	};
}