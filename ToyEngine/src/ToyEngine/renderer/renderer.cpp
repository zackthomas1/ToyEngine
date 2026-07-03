#include "pch.h"
#include "renderer.h"
#include "ToyEngine/skybox.h"

namespace ToyEngine
{
  // Initialize the static instance of the Renderer class to nullptr.
  // This ensures that there is only one instance of the Renderer throughout the application.
  Renderer* Renderer::s_instance = nullptr;

  Renderer::Renderer(eRenderAPI api)
    : api_(api) {}

  void Renderer::Init()
  {
#ifdef TY_PLATFORM_OPENGL
    eRenderAPI api = eRenderAPI::kOpenGL;
#else
    eRenderAPI api = eRenderAPI::kNone;
#endif TY_PLATFORM_OPENGL
    TY_CORE_ASSERT(!s_instance, "Renderer already exist!"); 
    s_instance = new Renderer(api);
    RenderAPI::Init(api);
  }

  void Renderer::BeginScene()
  {
    // Reset state for scene rendering.
    RenderCommand::Enable(eParamType::kDEPTH_TEST);

    RenderCommand::Enable(eParamType::kCULL_FACE);
    RenderCommand::CullFace(eParamType::kBACK);
    RenderCommand::FrontFace(eParamType::kCCW);

    RenderCommand::ClearSetBackground();
  }

  void Renderer::Submit(const VertexArray* vao)
  {
    vao->Bind();
    RenderCommand::DrawElements(ePrimType::kTRIANGLE, vao);
    vao->Unbind();
  }

  void Renderer::Submit(SceneNode* node)
  {
    if(!node) return;

    node->UpdateWorldTransform();

    // Render non-skybox entities first
    if (node->GetEntity() && !dynamic_cast<Skybox*>(node->GetEntity().get()))
      node->GetEntity()->Render(node->GetWorldTransform());

    for (const Scope<SceneNode>& child : node->GetChildren()) {
      Renderer::Submit(child.get());
    }

    // Render skybox entities last
    if (node->GetEntity() && dynamic_cast<Skybox*>(node->GetEntity().get()))
      node->GetEntity()->Render(node->GetWorldTransform());
  }

  void Renderer::EndScene() { }
}