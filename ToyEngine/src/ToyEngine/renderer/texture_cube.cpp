#include "pch.h"
#include "texture_cube.h"
#include "ToyEngine/platform/opengl/opengl_texture_cube.h"
#include "ToyEngine/renderer/renderer.h"

namespace ToyEngine
{
	TextureCube::TextureCube(const Array<std::string, 6>& face_paths) :
		m_face_paths(face_paths), m_is_single_file(false)
	{}

	TextureCube::TextureCube(const std::string& path) :
		m_single_path(path), m_is_single_file(true)
	{}

	Ref<TextureCube> TextureCube::Create(const Array<std::string, 6>& face_paths, bool flip_vertically)
	{
		switch (Renderer::API())
		{
		case(eRenderAPI::kOpenGL): {
			return MakeRef<OpenGLTextureCube>(face_paths, flip_vertically);
		}
		default:
			TY_CORE_ERROR("Unknown render api");
			return nullptr;
		}
	}

	Ref<TextureCube> TextureCube::Create(const std::string& path, bool flip_vertically)
	{
		switch (Renderer::API())
		{
		case(eRenderAPI::kOpenGL): {
			return MakeRef<OpenGLTextureCube>(path, flip_vertically);
		}
		default:
			TY_CORE_ERROR("Unknown render api");
			return nullptr;
		}
	}
}