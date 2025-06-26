#pragma once
#include "ToyEngine/renderer/texture/texture_2d.h"
#include "ToyEngine/renderer/shader_s.h"

namespace ToyEngine
{
	class Material
	{
	public:
		Material();
		virtual ~Material() {}
	
		void SetMaterialUniforms(Shader* shader);

		std::vector<std::shared_ptr<Texture2D>>* GetTextures() { return &textures_; }
		void AddTexture(std::shared_ptr<Texture2D> texture) { textures_.push_back(texture); }
		void ActivateTextureUnits();
	private:
		std::vector<std::shared_ptr<Texture2D>> textures_;
	};
}