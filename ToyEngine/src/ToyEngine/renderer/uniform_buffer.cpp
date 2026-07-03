#include "pch.h"
#include "uniform_buffer.h"
#include "ToyEngine/renderer/renderer.h"
#include "ToyEngine/platform/opengl/opengl_uniform_buffer.h"

namespace ToyEngine {
  uint32_t UniformBuffer::s_next_binding_point = 0;

  Ref<UniformBuffer> UniformBuffer::Create(const std::string& name, uint32_t size)
  {
    switch (Renderer::API())
    {
    case(eRenderAPI::kOpenGL): {
      return MakeRef<OpenGLUniformBuffer>(name, size, s_next_binding_point++);
    }
    default:
      TY_CORE_ERROR("Render API unknown");
      return nullptr;
    }
  }
}