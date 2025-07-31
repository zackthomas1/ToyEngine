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
		data_.m_width				= props.m_width;
		data_.m_height				= props.m_height;
		data_.m_multisampling		= props.m_multisampling;
		data_.m_samples				= props.m_samples;
		data_.m_depth_attachment	= props.m_depth_attachment;
		data_.m_stencil_attachment	= props.m_stencil_attachment;
	}
}