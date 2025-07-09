#include "pch.h"
#include "texture_2d.h"
#include "ToyEngine/platform/opengl/opengl_texture_2d.h"
#include "ToyEngine/renderer/renderer.h"

namespace ToyEngine
{
	Texture2D::Texture2D(const std::string& path, eTextureType type) :
		m_path(path), m_type(type)
	 {}

	Ref<Texture2D> Texture2D::Create(const std::string& path, eTextureType type, bool flip_vertically)
	{
		switch (Renderer::API())
		{
		case(eRenderAPI::kOpenGL): {
			return MakeRef<OpenGLTexture2D>(path, type, flip_vertically);
		}
		default:
			TY_CORE_ERROR("Unknown render api");
			return nullptr;
		}
	}
}
