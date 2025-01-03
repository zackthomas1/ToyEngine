#pragma once
#include "ToyEngine/renderer/shader_s.h"

namespace ToyEngine
{
	class RenderAPI
	{
	public:
		static void ShaderUse(Shader* shader);
		static void SetFloat(Shader* shader, const std::string& name, float value);
	};
}