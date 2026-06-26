#pragma once
#include "ToyEngine/renderer/frame_buffer.h"

namespace ToyEngine
{
  class OpenGLFrameBuffer : public FrameBuffer
  {
  public:
    OpenGLFrameBuffer(const FrameBufferProps& props);
    ~OpenGLFrameBuffer();

    virtual void Bind() override;
    virtual void Unbind() override;
    virtual void CopyColorBuffer() const override {TY_CORE_WARN("TODO - implement CopyColorBuffer method in OpenGLFrameBuffer class"); }
    virtual void CopyDepthBuffer() const override { TY_CORE_WARN("TODO - implement CopyDepthBuffer method in OpenGLFrameBuffer class"); }
    virtual void CopyStencilBuffer() const override { TY_CORE_WARN("TODO - implement CopyStencilBuffer method in OpenGLFrameBuffer class"); }
    virtual void Resize(uint32_t width, uint32_t height) override;
    virtual void Status() const override;

    virtual uint32_t GetColorAttachment() const { return color_attachment_id_; }

  private:
    void CreateBuffers(uint32_t width, uint32_t height);
    void DeleteBuffers();
    uint32_t id_;
    uint32_t color_attachment_id_, depth_stencil_attachment_id_;
  };
}