#include "pch.h"
#include "render_api.h"

#include <glad/glad.h>

namespace ToyEngine
{
	void RenderAPI::ShaderUse(Shader* shader)
	{
		glUseProgram(shader->id());
	}

	void RenderAPI::SetFloat(Shader* shader, const std::string& name, float value)
	{
		glUniform1f(glGetUniformLocation(shader->id(), name.c_str()), value);
	}
}

