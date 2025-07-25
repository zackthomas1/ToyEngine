#pragma once
#include "ToyEngine/renderer/texture_cube.h"
#include "ToyEngine/platform/opengl/opengl_texture_2d.h"

namespace ToyEngine {
	class OpenGLTextureCube : public TextureCube
	{
	public:
		OpenGLTextureCube(Array<std::string, CUBE_SIDES>& files); 
		~OpenGLTextureCube() {};

		virtual void Bind(uint32_t slot = 0) const override; 
		virtual void GenerateMipMaps() override;
	private:
		int width_ = 0, height_ = 0, nrChannels_ = 0;
	};
}