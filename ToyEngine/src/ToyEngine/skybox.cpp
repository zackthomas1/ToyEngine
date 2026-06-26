#include "pch.h"
#include "skybox.h"
#include "renderer/renderer.h"
#include "ToyEngine/model/primatives.h"
namespace ToyEngine
{
  Skybox::Skybox(Ref<TextureCube> texture, Ref<Shader> shader) : texture_(texture), m_shader(shader)
  {
    TY_CORE_ASSERT(texture_, "Skybox - Null TextureCube provided");
    TY_CORE_ASSERT(shader, "Skybox - Null Shader provided");

    m_vao = VertexArray::Create();
    Ref<VertexBuffer> buffer = VertexBuffer::Create(SkyBoxPrim::m_vertices.data(), sizeof(SkyBoxPrim::m_vertices));
    buffer->SetLayout(SkyBoxPrim::m_layout);
    Ref<IndexBuffer> indices = IndexBuffer::Create(SkyBoxPrim::m_indices.data(), SkyBoxPrim::m_indices.size());
    m_vao->AddBuffer(buffer);
    m_vao->SetIndexBuffer(indices);
  }

  Skybox::~Skybox() {}

  void Skybox::Render(const glm::mat4& world_transform) const
  {
    // validation
    TY_CORE_ASSERT(m_shader, "Skybox - Null Shader");
    TY_CORE_ASSERT(texture_, "Skybox - Null TextureCube");
    TY_CORE_ASSERT(m_vao != 0, "Skybox - Invalid VAO");

    // Save current OpenGL state
    bool depthMask;
    RenderCommand::GetBooleanv(eParamType::kDEPTH_WRITEMASK, &depthMask);
    bool cullFace;
    RenderCommand::GetBooleanv(eParamType::kCULL_FACE, &cullFace);
    int depthFunc;
    RenderCommand::GetIntegerv(eParamType::kDEPTH_FUNC, &depthFunc);

    // Set skybox rendering state
    RenderCommand::DepthMask(false);
    RenderCommand::Disable(eParamType::kCULL_FACE);
    RenderCommand::DepthFunc(eDepthFunc::kLEQUAL);

    m_shader->Use();
    m_shader->SetInt("skybox_texture", 0);
    texture_->Bind(0);
    Renderer::Submit(m_vao.get());

    // Restore previous OpenGL state
    RenderCommand::DepthMask(depthMask);
    if (cullFace) RenderCommand::Enable(eParamType::kCULL_FACE);
    RenderCommand::DepthFunc((eDepthFunc)depthFunc);
  }
}