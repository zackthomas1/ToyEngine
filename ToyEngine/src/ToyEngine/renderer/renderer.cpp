#include "pch.h"
#include "renderer.h"

#include "ToyEngine/platform/opengl/render_api.h"
#include "ToyEngine/services/locator.h"

namespace ToyEngine
{
	Renderer* Renderer::s_instance = nullptr; 

	Renderer::Renderer(eRenderAPI api, SceneData data) : api_(api), m_data(data)
	{
	}

	Renderer::~Renderer() {}

	void Renderer::BeginScene(Ref<Camera> camera)
	{
		RenderAPI::ClearSetBackground();
		s_instance->m_data.view = camera->GetViewMatrix();
		s_instance->m_data.projection = camera->GetProjectionMatrix();
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
			RenderAPI::BindVertexArray(mesh->m_vao);
			RenderAPI::DrawIndexed(mesh->m_indices.size());
			RenderAPI::BindVertexArray(0);
		}
	}

	void Renderer::EndScene() {}

	void Renderer::Init(eRenderAPI api)
	{
		TY_CORE_ASSERT(!s_instance, "Renderer already exist!")
		s_instance = new Renderer(api);
	}
}