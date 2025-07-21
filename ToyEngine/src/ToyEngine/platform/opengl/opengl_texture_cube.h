#pragma once
#include "ToyEngine/renderer/texture_cube.h"

namespace ToyEngine
{
	class OpenGLTextureCube : public TextureCube
	{
	public:
		OpenGLTextureCube(const Array<std::string, 6>& face_paths, bool flip_vertically = false);
		OpenGLTextureCube(const std::string& path, bool flip_vertically = false);
		~OpenGLTextureCube();

		virtual void Bind(uint32_t texture_unit) const override;
	};
}