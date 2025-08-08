#include "pch.h"
#include "texture_cube.h"
#include "ToyEngine/renderer/renderer.h"
#include "ToyEngine/platform/opengl/opengl_texture_cube.h"

namespace ToyEngine {
	Ref<TextureCube> TextureCube::Create(const std::initializer_list<std::string>& files)
	{
		if(files.size() != CUBE_SIDES)
			TY_CORE_ERROR("TextureCube must be initialized with {} files {} files provided.", CUBE_SIDES, files.size());

		switch (Renderer::API())
		{
		case(eRenderAPI::kOpenGL): {
			return MakeRef<OpenGLTextureCube>(files);
		}
		default:
			TY_CORE_ERROR("Unknown render api");
			return nullptr;
		}
	}
}

