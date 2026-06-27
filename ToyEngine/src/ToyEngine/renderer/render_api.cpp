#include "pch.h"
#include "render_api.h"
#include "ToyEngine/platform/opengl/opengl_render_api.h"


namespace ToyEngine
{
  Scope<RenderAPI> RenderAPI::s_instance = nullptr;

  void ToyEngine::RenderAPI::Init(eRenderAPI api)
  {
    TY_CORE_ASSERT(!s_instance, "RenderAPI already initialized. To change backend graphics api, please shutdown the current instance first.");
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
