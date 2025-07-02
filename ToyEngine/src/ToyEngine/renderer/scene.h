#pragma once
#include "pch.h"

#include "ToyEngine/renderer/model.h"
#include "ToyEngine/renderer/light.h"
#include "ToyEngine/renderer/camera/camera.h"
#include "ToyEngine/services/time_step.h"

namespace ToyEngine
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		void Update(TimeStep *time_step);

		// scene modifiers
		void AddModel(std::shared_ptr<Model> model);
		//void AddLight(Light light); 

		// getter/setter
		const std::vector<std::shared_ptr<Model>>& GetModels() const { return models_; }
		const Camera* GetCamera() const { return camera_; }

	private:
		Camera* camera_;
		std::vector<std::shared_ptr<Model>> models_; 
		//std::vector<Light> lights_; 

	};
}