#pragma once
#include "pch.h"
#include "ToyEngine/renderer/model.h"
#include "ToyEngine/renderer/light.h"

#include "ToyEngine/renderer/camera/camera.h"

namespace ToyEngine
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		void Update(float time_delta);

		// scene modifiers
		void AddModel(std::shared_ptr<Model> model);
		//void AddLight(Light light); 

		// getter/setter
		const std::vector<std::shared_ptr<Model>>& GetModels() const { return models_; }
		const std::vector<std::shared_ptr<Camera>>& GetCameras() const { return cameras_; }

	private:
		std::vector<std::shared_ptr<Camera>> cameras_;
		std::vector<std::shared_ptr<Model>> models_; 
		//std::vector<Light> lights_; 

	};
}