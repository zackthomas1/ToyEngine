#pragma once
#include "ToyEngine/scene_node.h"
#include "ToyEngine/model/model.h"
#include "ToyEngine/renderer/light.h"
#include "ToyEngine/renderer/camera.h"
#include "ToyEngine/renderer/shader.h"
#include "ToyEngine/renderer/uniform_buffer.h"
#include "ToyEngine/renderer/render_command_queue.h"

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
        /// Begins a new rendering scene by setting up camera matrices and lighting data in the uniform buffer.
        /// This function prepares the renderer for drawing by clearing the background and updating
        /// the uniform buffer with the current camera's view and projection matrices, as well as the provided lighting information.
        /// </summary>
        /// <param name="camera">The camera to use for view and projection matrices.</param>
        /// <param name="light_block">Pointer to the LightBlock containing scene lighting data.</param>
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
		RenderCommandQueue m_command_queue;
	};
}