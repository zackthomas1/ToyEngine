#pragma once
#include "pch.h"
#include "ToyEngine/renderer/model.h"
#include "ToyEngine/renderer/light.h"

#include "ToyEngine/renderer/camera/fly_camera.h"

namespace ToyEngine
{
	class Scene
	{
	public:
		Scene();
		~Scene() {}
			
		std::vector<std::shared_ptr<Model>>* GetSceneModels() { return &models_; }

		void AddModel(std::shared_ptr<Model> model);
		//void AddLight(Light light); 

	private:
		std::vector<std::shared_ptr<Model>> models_; 
		//std::vector<Light> lights_; 

	};
}