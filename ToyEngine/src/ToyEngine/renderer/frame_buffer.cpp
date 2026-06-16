#include "pch.h"
#include "frame_buffer.h"
#include "ToyEngine/renderer/renderer.h"
#include "ToyEngine/platform/opengl/opengl_frame_buffer.h"

namespace ToyEngine
{
  Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferProps& props)
  {
    switch (Renderer::API())
    {
    case(eRenderAPI::kOpenGL): {
      return MakeRef<OpenGLFrameBuffer>(props);
    }
    default:
      TY_CORE_ERROR("Invalid Render API");
      return nullptr; 
    }
  }
  FrameBuffer::FrameBuffer(const FrameBufferProps& props)
  {
    data_.width = props.width;
    data_.height = props.height;
    data_.multisampling = props.multisampling;
    data_.samples = props.samples;
    data_.depth_attachment = props.depth_attachment;
    data_.stencil_attachment= props.stencil_attachment;
  }
}