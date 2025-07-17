#include "pch.h"
#include "renderer.h"
#include "ToyEngine/services/locator.h"
#include "ToyEngine/renderer/render_api.h"

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

		Renderer::GetUniformManager().CreateBuffer("ViewProjectMats", 2 * sizeof(glm::mat4) + sizeof(glm::vec3) + sizeof(float));
		Renderer::GetUniformManager().CreateBuffer("LightBlock", sizeof(LightBlock));
	}

	void Renderer::BeginScene(Ref<Camera> camera, const LightBlock* light_block)
	{
		// Clear the background to prepare for rendering a new frame.
		RenderCommand::ClearSetBackground();

		// Update uniform buffer objects
		Ref<UniformBuffer> camera_uniforms = Renderer::GetUniformManager().GetBuffer("ViewProjectMats");
		camera_uniforms->SetData(0, sizeof(glm::mat4), glm::value_ptr(camera->GetViewMatrix()));
		camera_uniforms->SetData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera->GetProjectionMatrix()));
		camera_uniforms->SetData(2 * sizeof(glm::mat4), sizeof(glm::vec3), glm::value_ptr(camera->position()));
		
		Ref<UniformBuffer> light_uniforms = Renderer::GetUniformManager().GetBuffer("LightBlock");
		TY_CORE_ASSERT(sizeof(LightBlock) == 1024, "LightBlock Incorrect size");
		light_uniforms->SetData(0, sizeof(LightBlock), light_block);
	}

	void Renderer::Submit(Ref<Model> model, const glm::mat4& world_transform)
	{
		Ref<Shader> shader = model->m_shader;
		shader->Use();
		for (Ref<Mesh> mesh : model->m_meshes) {
			shader->SetMat4("uModel", world_transform);
			mesh->m_material->BindTextures(shader);

			// draw mesh
			RenderCommand::BindVertexArray(mesh->m_vao);
			RenderCommand::DrawIndexed(mesh->m_indices.size());
			RenderCommand::BindVertexArray(0);
		}
	}

	void Renderer::Submit(SceneNode* scene)
	{
		if(!scene) return;
		scene->UpdateWorldTransform();
		if(scene->GetEntity())
			Renderer::Submit(scene->GetEntity(), scene->GetWorldTransform());

		for (const Scope<SceneNode>& child : scene->GetChildren()) {
			Renderer::Submit(child.get());
		}
	}

	void Renderer::EndScene() {}
}