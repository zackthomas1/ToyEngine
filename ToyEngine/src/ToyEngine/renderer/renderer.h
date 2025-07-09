#pragma once

#include "ToyEngine/renderer/camera.h"
#include "ToyEngine/renderer/model.h"
#include "ToyEngine/platform/opengl/shader_s.h"

namespace ToyEngine
{
	class Renderer
	{
	public:

		static void BeginScene(Ref<Camera> camera);
		static void Submit(Ref<Shader> shader, Ref<Model> model);
		static void EndScene();
		static void Init(eRenderAPI api = eRenderAPI::kOpenGL);

		static Renderer* s_instance;
		static eRenderAPI API() { return Renderer::s_instance->api_; }

		struct SceneData{
			glm::mat4 view;
			glm::mat4 projection;
			
			SceneData () : view(glm::mat4(1.0f)), projection(glm::mat4(1.0f)) {}
		};
		SceneData m_data;
	protected:
		Renderer(eRenderAPI api, SceneData data = SceneData());
		~Renderer();
	private:
		eRenderAPI api_;
	};
}