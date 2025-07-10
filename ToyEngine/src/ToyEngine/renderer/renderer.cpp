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
	}

	void Renderer::BeginScene(Ref<Camera> camera)
	{
		RenderCommand::ClearSetBackground();
		//s_instance->m_data.view = camera->GetViewMatrix();
		//s_instance->m_data.projection = camera->GetProjectionMatrix();
	}

	void Renderer::Submit(Ref<Shader> shader, Ref<Model> model)
	{
		shader->Use();
		shader->SetMat4("uView", s_instance->m_data.view);
		shader->SetMat4("uProjection", s_instance->m_data.projection);
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