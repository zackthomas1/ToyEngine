#include "pch.h"
#include "vertex_array.h"
#include "ToyEngine/renderer/renderer.h"
#include "ToyEngine/platform/opengl/opengl_vertex_array.h"

namespace ToyEngine
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::API())
		{
		case(eRenderAPI::kOpenGL): {
			return MakeRef<OpenGLVertexArray>();
		}
		default:
			TY_CORE_ERROR("Unknown Render API");
			return nullptr;
		}
	}
}