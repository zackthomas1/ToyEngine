#include "pch.h"
#include "material.h"
#include "ToyEngine/renderer/shader.h"
#include <glad/glad.h>

namespace ToyEngine
{

	Ref<TextureCube> Material::s_default_environment_map = nullptr;

	Ref<TextureCube> Material::GetDefaultEnvironmentMap()
	{
		if (!s_default_environment_map) {
			TY_CORE_WARN("Using default environmnet map: "
				"OpenGL requires that all active samplers in shaderhave valid texture bound to texture units,"
				"regardless of whether they are sampled in shader logic."
				"OpenGL still validates that the sampler has a bound texture during a draw call"
			);

			ToyEngine::Array<std::string, 6> files = {
				"../assets/cubemaps/starfield/nx.png",	// +X (right)
				"../assets/cubemaps/starfield/ny.png",	// -X (left)
				"../assets/cubemaps/starfield/nz.png",	// +Y (top)
				"../assets/cubemaps/starfield/px.png",	// -Y (bottom)
				"../assets/cubemaps/starfield/py.png",	// +Z (front)
				"../assets/cubemaps/starfield/pz.png",	// -Z (back)
			};
			s_default_environment_map = ToyEngine::TextureCube::Create(files);
		}
		return s_default_environment_map;
	}

	void Material::Bind(Ref<Shader> shader)
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
		}

		// Bind environemnt map 
		if (environment_map_) {
			environment_map_->Bind(textures_.size());
			shader->SetInt("uMaxMipLevel", std::max(0, environment_map_->GeMaxMipLevel()));
			shader->SetInt("material.environment_map", textures_.size());
			shader->SetBool("material.has_environment_map", true);
		}
		else {
			GetDefaultEnvironmentMap()->Bind(textures_.size());
			shader->SetInt("uMaxMipLevel", std::max(0, GetDefaultEnvironmentMap()->GeMaxMipLevel()));
			shader->SetInt("material.environment_map", textures_.size());
			shader->SetBool("material.has_environment_map", false);
		}

		// set material properties
		//TY_CORE_INFO("{}", transmission_);
		shader->SetFloat("material.roughness", roughness_);
		shader->SetFloat("material.metallic", metallic_);
		shader->SetFloat("material.transmission", transmission_);
		if(refractive_index_ < TY_EPSILON) TY_CORE_WARN(" Division by zero: refractive index equal to zero.");
		shader->SetFloat("material.refractive_index", refractive_index_);
	}
}