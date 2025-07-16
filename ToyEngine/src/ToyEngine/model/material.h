#pragma once
#include "ToyEngine/renderer/texture_2d.h"
#include "ToyEngine/renderer/shader.h"

namespace ToyEngine
{
	class Material
	{
	public:
		Material() : shininess_(32.0f) { TY_CORE_WARN("Material textures empty."); }
		Material(const Vector<Ref<Texture2D>>& textues) : shininess_(32.0f), textures_(textues) {}

		virtual ~Material() {}
		const Vector<Ref<Texture2D>>& getTextures() const { return textures_; }
		void BindTextures(Ref<Shader> shader);
	private:
		float shininess_;
		Vector<Ref<Texture2D>> textures_;
	};
}