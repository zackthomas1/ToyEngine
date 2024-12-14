#pragma once
#include "ToyEngine/renderer/texture/texture_2d.h"


namespace ToyEngine
{
	class Material
	{
	public:
		Material();
		virtual ~Material() {}
	
		std::vector<std::shared_ptr<Texture2D>>* GetTextures() { return &textures_; }
		void AddTexture(std::shared_ptr<Texture2D> texture) { textures_.push_back(texture); }
		void ActivateTextureUnits();
	private:
		std::vector<std::shared_ptr<Texture2D>> textures_;
	};
}