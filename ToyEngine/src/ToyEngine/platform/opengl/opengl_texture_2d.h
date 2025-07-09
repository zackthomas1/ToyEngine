#pragma once
#include "ToyEngine/renderer/texture_2d.h"

namespace ToyEngine
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(std::string &path, eTextureType type, bool flip_vertically = false);
		~OpenGLTexture2D();

		virtual void Bind(uint32_t texture_unit) const override;
		void SetParameters(uint32_t wrap_s, uint32_t wrap_t, uint32_t min_filter, uint32_t mag_filter);
	};
}