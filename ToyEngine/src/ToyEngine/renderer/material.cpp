#include "pch.h"
#include "material.h"
#include "ToyEngine/platform/opengl/shader_s.h"
#include <glad/glad.h>

namespace ToyEngine
{
	Material::Material()
	{
	}

	Material::Material(const Vector<Ref<Texture2D>> &textues) :
		textures_(textues)
	{
	}

	void Material::BindTextures(Ref<Shader> shader)
	{
		// bind texture on corresponding texture units
		unsigned int i = 0, diffuseNR = 1, specularNR = 1;
		for (Ref<Texture2D> texture : getTextures()) {
			texture->Bind(i);
			switch (texture->m_type) {
			case(eTextureType::kDiffuse): {
				std::string s = "material." + std::string("texture_diffuse") + std::to_string(diffuseNR++);
				shader->SetInt(s, i++);
				break;
			}
			case(eTextureType::kSpecular): {
				std::string s = "material." + std::string("texture_specular") + std::to_string(specularNR++);
				shader->SetInt(s, i++);
				break;
			}
			}
			texture->Unbind();
		}
	}
}