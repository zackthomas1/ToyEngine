#pragma once
#include "ToyEngine/renderer/texture_2d.h"
#include "ToyEngine/renderer/shader.h"

namespace ToyEngine
{
	class Material
	{
	public:
		Material();
		Material(const Vector<Ref<Texture2D>>& textues);

		virtual ~Material() {}
		const Vector<Ref<Texture2D>>& getTextures() const { return textures_; }
		void BindTextures(Ref<Shader> shader);
	private:
		Vector<Ref<Texture2D>> textures_;
	};
}