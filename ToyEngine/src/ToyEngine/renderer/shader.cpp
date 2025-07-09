#include "pch.h"
#include "shader.h"
#include "ToyEngine/renderer/renderer.h"
#include "ToyEngine/platform/opengl/opengl_shader.h"

namespace ToyEngine {
	Ref<Shader> Shader::Create(const char* shader_name, const char* vertex_path, const char* fragment_path)
	{
		switch (Renderer::API())
		{
		case(eRenderAPI::kOpenGL): {
			return MakeRef<OpenGLShader>(shader_name, vertex_path, fragment_path);
		}
		default:
			TY_CORE_ERROR("Unknown render api");
			return nullptr;
		}
	}

	void ShaderLibrary::Load(Ref<Shader> shader)
	{
		library_.emplace(std::make_pair(std::string(shader->m_name), shader));
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		auto it = library_.find(name);
		Ref<Shader> shader;
		if (it != library_.end()) {
			shader = it->second;
		}
		TY_CORE_ASSERT(shader, "Shader library returned NULL. Shader not found.")
			return shader;
	}
}