#pragma once
#include "pch.h"
#include "ToyEngine/renderer/model.h"
#include "ToyEngine/renderer/light.h"

namespace ToyEngine
{
	class Scene
	{
	public:
		Scene() {}
		~Scene() {}
		
		void DrawScene();
		
		void AddModel(Model model);
		//void AddLight(Light light); 

	private:
		std::vector<Model> models_; 
		//std::vector<Light> lights_; 

	};
}