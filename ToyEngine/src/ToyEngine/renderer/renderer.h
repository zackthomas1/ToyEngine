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
		~Renderer() {}

		void DrawScene(std::shared_ptr<Scene> scene);

		// getter/setters 
		void SetCamera(std::shared_ptr<Camera> camera) { render_camera_ = camera; }

		// Observer methods
		virtual void OnNotify(Event& event);

		static std::unique_ptr<Renderer> Create();
	private: 
		std::shared_ptr<Camera> render_camera_;
		std::shared_ptr<Shader> shader_;
	};
}