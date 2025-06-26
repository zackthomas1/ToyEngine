#include "pch.h"
#include "material.h"

#include <glad/glad.h>

namespace ToyEngine
{
	Material::Material()
	{
		// generate textures
		// ---------
		std::shared_ptr<Texture2D> container_tex = std::make_shared<Texture2D>("../assets/textures/container.jpg");
		std::shared_ptr<Texture2D> awesome_face_tex = std::make_shared<Texture2D>("../assets/textures/awesomeface.png", true);
		awesome_face_tex->SetParameters(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

		AddTexture(container_tex);
		AddTexture(awesome_face_tex);
	}

	void Material::SetMaterialUniforms(Shader* shader)
	{
		for (int i = 0; i < GetTextures()->size(); i++) {
			int sampler_location = ((*GetTextures())[i]->id()) - 1;
			//TY_CORE_INFO("SetMaterialUniforms - name:{}", "text_sample_" + std::to_string(sampler_location));
			shader->SetInt("text_sample_" + std::to_string(sampler_location), sampler_location);
		}
	}

	void Material::ActivateTextureUnits()
	{
		// bind texture on corresponding texture units
		for (int i = 0; i < textures_.size(); i++) {
			textures_[i]->Activate();
		}
	}
}
