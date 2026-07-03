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
}