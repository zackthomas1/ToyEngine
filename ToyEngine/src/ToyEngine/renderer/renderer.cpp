#include "pch.h"
#include "renderer.h"

#include "ToyEngine/renderer/render_api.h"
#include "ToyEngine/services/locator.h"

namespace ToyEngine
{
	Renderer* Renderer::s_instance = nullptr; 

	Renderer::Renderer(eRenderAPI api, SceneData data) : api_(api), m_data(data)
	{
	}

	void Renderer::Init()
	{
#ifdef  TY_PLATFORM_OPENGL
		eRenderAPI api = eRenderAPI::kOpenGL;
#else
		eRenderAPI api = eRenderAPI::kNone;
#endif TY_PLATFORM_OPENGL
		TY_CORE_ASSERT(!s_instance, "Renderer already exist!"); 
		s_instance = new Renderer(api);
		RenderAPI::Init(api);

		UniformManager& manager = Renderer::GetUniformManager();
		manager.Add("ViewProjectMats", UniformBuffer::Create(2 * sizeof(glm::mat4), manager.GetNextBindingPoint()));
	}

	void Renderer::BeginScene(Ref<Camera> camera)
	{
		// Clear the background to prepare for rendering a new frame.
		RenderCommand::ClearSetBackground();

		// Set the view and projection matrix data in uniform buffer.
		ToyEngine::Ref<ToyEngine::UniformBuffer> matrix_buffer = ToyEngine::Renderer::GetUniformManager().GetBuffer("ViewProjectMats");
		matrix_buffer->SetData(0, sizeof(glm::mat4), glm::value_ptr(camera->GetViewMatrix()));
		matrix_buffer->SetData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera->GetProjectionMatrix()));
	}

	void Renderer::Submit(Ref<Shader> shader, Ref<Model> model)
	{
		shader->Use();
		for (Ref<Mesh> mesh : model->m_meshes) {
			shader->SetMat4("uModel", model->m_model_mat);
			mesh->m_material->BindTextures(shader);

			// draw mesh
			RenderCommand::BindVertexArray(mesh->m_vao);
			RenderCommand::DrawIndexed(mesh->m_indices.size());
			RenderCommand::BindVertexArray(0);
		}
	}

	void Renderer::EndScene() {}
}