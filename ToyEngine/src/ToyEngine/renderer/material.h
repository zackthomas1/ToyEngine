#pragma once
#include "ToyEngine/platform/opengl/texture_2d.h"
#include "ToyEngine/platform/opengl/shader_s.h"
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