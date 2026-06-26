#pragma once
#include "ToyEngine/renderer/uniform_buffer.h"

namespace ToyEngine {
  class OpenGLUniformBuffer : public UniformBuffer
  {
  public:
    OpenGLUniformBuffer(uint32_t size, uint32_t binding_point);
    ~OpenGLUniformBuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void SetData(uint32_t offset, uint32_t size, const void* data) override;
    virtual void BindToBindingPoint(uint32_t binding_point) const override;
  };
}