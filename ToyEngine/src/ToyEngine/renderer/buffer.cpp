#include "pch.h"
#include "buffer.h"
#include "ToyEngine/renderer/renderer.h"
#include "ToyEngine/platform/opengl/opengl_buffer.h"

namespace ToyEngine {
    BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
		: elements_(elements)
	{
		CalculateOffsetAndStride();
	}

	void BufferLayout::CalculateOffsetAndStride()
	{
		uint32_t offset = 0; 
		stride_ = 0; 
		for (auto& element : elements_) 
		{
			element.m_offset	= offset; 
			offset				+= element.m_size; 
			stride_				+= element.m_size;
		}
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* data, uint32_t size)
	{
		switch (Renderer::API())
		{
		case eRenderAPI::kOpenGL: {
			return MakeRef<OpenGLVertexBuffer>(data, size);
		}
		default:
			TY_CORE_ERROR("Unknow Render API");
			return nullptr; 
		}
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* data, uint32_t count)
	{
		switch (Renderer::API())
		{
		case eRenderAPI::kOpenGL: {
			return MakeRef<OpenGLIndexBuffer>(data, count);
		}
		default:
			TY_CORE_ERROR("Unknow Render API");
			return nullptr;
		}
	}
}