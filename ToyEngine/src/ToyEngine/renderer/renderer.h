#pragma once
#include "ToyEngine/scene_node.h"
#include "ToyEngine/model/model.h"
#include "ToyEngine/renderer/light.h"
#include "ToyEngine/renderer/camera.h"
#include "ToyEngine/renderer/shader.h"
#include "ToyEngine/renderer/uniform_buffer.h"

namespace ToyEngine
{
	struct SceneData {
		SceneData() {}
	};

	class Renderer
	{
	public:
		static void Init();

		/// <summary>
		/// Begins a new rendering scene by setting up camera matrices in the uniform buffer.
		/// This function prepares the renderer for drawing by clearing the background and updating
		/// the uniform buffer with the current camera's view and projection matrices.
		/// </summary>
		/// <param name="camera"></param>
		static void BeginScene(Ref<Camera> camera, const LightBlock* light_block);
		static void Submit(Ref<Model> model, const glm::mat4& world_transform);
		static void Submit(SceneNode* scene);
		static void EndScene();

		static eRenderAPI API() { return Renderer::s_instance->api_; }
		static UniformManager& GetUniformManager() { return Renderer::s_instance->m_uniform_manager; }

	protected:
		Renderer(eRenderAPI api, SceneData data = SceneData());
		~Renderer() {}
	private:
		static Renderer* s_instance;
		eRenderAPI api_;
		UniformManager m_uniform_manager;
		SceneData m_data;
	};
}