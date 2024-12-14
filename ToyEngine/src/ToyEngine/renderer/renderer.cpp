#include "pch.h"
#include "renderer.h"

#include "ToyEngine/renderer/mesh/primatives/plane.h"
#include "ToyEngine/renderer/mesh/primatives/cube.h"
#include "ToyEngine/renderer/mesh/primatives/triangle.h"


#include <glad/glad.h>

namespace ToyEngine
{
	Renderer::Renderer(std::shared_ptr<Scene> scene) : scene_(scene)
	{
		std::shared_ptr<Shader> shader = std::make_shared<Shader>("../assets/shaders/1_6_textures.vs", "../assets/shaders/1_6_textures.fs");
		std::shared_ptr<Cube> cube = std::make_shared<Cube>();
		std::shared_ptr<Material> material = std::make_shared<Material>();
		
		model_ = Model(shader, cube, material);

		model_.GetShader()->Use();
		model_.GetShader()->SetFloat3("pickedColor", 1.0f, 0.0f, 0.0f);
		model_.SetMaterialUniforms();
	}

	void Renderer::DrawScene()
	{
        // render
        // ------
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		Draw(model_);
		//scene_->DrawScene();
	}

	void Renderer::Draw(Model model)
	{
		model.GetShader()->Use();
		model.GetMaterial()->ActivateTextureUnits();

		model.GetMesh()->Draw();
	}

	std::unique_ptr<Renderer> Renderer::Create(std::shared_ptr<Scene> scene)
	{
		return std::make_unique<Renderer>(scene);
	}
}