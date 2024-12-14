#pragma once
#include "ToyEngine/renderer/camera/camera.h"
#include "ToyEngine/renderer/camera/fly_camera.h"
#include "ToyEngine/renderer/scene.h"
#include "ToyEngine/renderer/shader_s.h"

namespace ToyEngine
{
	class Renderer
	{
	public:
		Renderer(std::shared_ptr<Scene> scene);
		~Renderer() {}

		void SetScene(std::shared_ptr<Scene> scene) { scene_ = scene; };
		void DrawScene();

		static std::unique_ptr<Renderer> Create(std::shared_ptr<Scene> scene);
	private: 
		void Draw(Model model);
	private: 
		Model model_;
		std::shared_ptr<Scene> scene_;
		//std::shared_ptr<Shader> shader_;
		//std::shared_ptr<FlyCamera> camera_;
	};
}