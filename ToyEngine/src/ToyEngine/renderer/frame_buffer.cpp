#include "pch.h"
#include "frame_buffer.h"
#include "ToyEngine/renderer/renderer.h"
#include "ToyEngine/platform/opengl/opengl_frame_buffer.h"

namespace ToyEngine
{
	Ref<FrameBuffer> FrameBuffer::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::API())
		{
		case(eRenderAPI::kOpenGL): {
			return MakeRef<OpenGLFrameBuffer>(width, height);
		}
		default:
			TY_CORE_ERROR("Invalid Render API");
			return nullptr; 
		}
	}
}