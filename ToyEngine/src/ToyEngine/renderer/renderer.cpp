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
		s_instance->m_matrices_buffer = MakeRef<MatricesBuffer>();
		RenderAPI::Init(api);
	}

	void Renderer::BeginScene(Ref<Camera> camera)
	{
		RenderCommand::ClearSetBackground();
		s_instance->m_data.view = camera->GetViewMatrix();
		s_instance->m_data.projection = camera->GetProjectionMatrix();
		
		// Update uniform buffer with camera matrices
		s_instance->m_matrices_buffer->SetViewMatrix(s_instance->m_data.view);
		s_instance->m_matrices_buffer->SetProjectionMatrix(s_instance->m_data.projection);
		s_instance->m_matrices_buffer->UpdateViewProjection();
		s_instance->m_matrices_buffer->Bind();
	}

	void Renderer::Submit(Ref<Shader> shader, Ref<Model> model)
	{
		shader->Use();
		// Set model matrix in uniform buffer
		s_instance->m_matrices_buffer->SetModelMatrix(model->m_model_mat);
		s_instance->m_matrices_buffer->UpdateModel();
		
		for (Ref<Mesh> mesh : model->m_meshes) {
			mesh->m_material->BindTextures(shader);

			// draw mesh
			RenderCommand::BindVertexArray(mesh->m_vao);
			RenderCommand::DrawIndexed(mesh->m_indices.size());
			RenderCommand::BindVertexArray(0);
		}
	}

	void Renderer::EndScene() {}
}