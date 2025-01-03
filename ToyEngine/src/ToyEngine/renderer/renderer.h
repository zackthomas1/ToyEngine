#pragma once
#include "ToyEngine/events/observer.h"

#include "ToyEngine/renderer/camera/camera.h"
#include "ToyEngine/renderer/camera/fly_camera.h"
#include "ToyEngine/renderer/scene.h"
#include "ToyEngine/renderer/shader_s.h"

namespace ToyEngine
{
	class Renderer : public Observer
	{
	public:
		Renderer();
		~Renderer();

		void DrawScene(std::shared_ptr<Scene> scene);

		// getter/setters 
		void SetRenderCamera(Camera* camera) { render_camera_ = camera; }

		// Observer methods
		virtual void OnEvent(Event& event) override;

		static std::unique_ptr<Renderer> Create();
	private: 
		Camera* render_camera_;
		Shader* shader_;
	};
}