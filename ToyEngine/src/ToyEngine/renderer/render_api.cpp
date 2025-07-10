#include "pch.h"
#include "render_api.h"
#include "ToyEngine/platform/opengl/opengl_render_api.h"


namespace ToyEngine
{
	Scope<RenderAPI> RenderAPI::s_instance = nullptr;

	void ToyEngine::RenderAPI::Init(eRenderAPI api)
	{
		switch (api)
		{
		case(eRenderAPI::kOpenGL): {
			s_instance = MakeScope<OpenGLRenderAPI>();
			break;
		}
		default:
			TY_CORE_ERROR("Unknown render api");
			s_instance = nullptr;
			break;
		}
	}
}
