#include "pch.h"
#include "renderer.h"

#include "ToyEngine/renderer/render_api.h"
#include "ToyEngine/services/locator.h"

namespace ToyEngine
{
	Renderer::Renderer()
	{
		shader_ = new Shader("../assets/shaders/1_9_camera.vs", "../assets/shaders/1_9_camera.fs");
	}

	Renderer::~Renderer()
	{
		delete shader_;
	}

	void Renderer::DrawScene(SceneLayer* scene)
	{
		RenderAPI::ClearSetBackground();
		RenderAPI::ShaderUse(shader_);

		const Camera *camera = scene->camera_;
		// send camera data to vertex shader
		shader_->SetMat4("u_view", camera->GetViewMatrix());
		shader_->SetMat4("u_projection", camera->GetProjectionMatrix());

		// draw each model in the scene
		for (std::shared_ptr<Model> model : scene->models_)
		{
			// activate shader and texture unites
			model->GetMaterial()->SetMaterialUniforms(shader_);
			model->GetMaterial()->ActivateTextureUnits();
		
			// texture related unifroms 
			shader_->SetFloat("u_alpha_tex", 0.5f);
			shader_->SetFloat("u_scale_tex", 1.5f);
			shader_->SetFloat2("u_pos_tex", 0.5f, 0.5f);

			// set model transforms in vertex shader
			shader_->SetMat4("u_model", model->GetModelMatrix());
			
			// draw mesh
			RenderAPI::DrawArrays(model->GetMesh());
		}
	}

	std::unique_ptr<Renderer> Renderer::Create()
	{
		return std::make_unique<Renderer>();
	}
}