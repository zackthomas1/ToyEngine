#include "pch.h"
#include "vertex_buffer.h"
#include "ToyEngine/renderer/renderer.h"
#include "ToyEngine/platform/opengl/opengl_vertex_buffer.h"

namespace ToyEngine
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::API())
		{
		case eRenderAPI::kOpenGL:
			return MakeRef<OpenGLVertexBuffer>(size);
		default:
			TY_CORE_ERROR("Render API unknown");
			return nullptr;
		}
	}

	Ref<VertexBuffer> VertexBuffer::Create(const void* data, uint32_t size)
	{
		switch (Renderer::API())
		{
		case eRenderAPI::kOpenGL:
			return MakeRef<OpenGLVertexBuffer>(data, size);
		default:
			TY_CORE_ERROR("Render API unknown");
			return nullptr;
		}
	}

	Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, uint32_t count)
	{
		switch (Renderer::API())
		{
		case eRenderAPI::kOpenGL:
			return MakeRef<OpenGLIndexBuffer>(indices, count);
		default:
			TY_CORE_ERROR("Render API unknown");
			return nullptr;
		}
	}
}