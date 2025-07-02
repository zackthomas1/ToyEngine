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
		Renderer();
		~Renderer();

		void DrawScene(std::shared_ptr<Scene> scene);

		static std::unique_ptr<Renderer> Create();
	private: 
		Shader* shader_;
	};
}