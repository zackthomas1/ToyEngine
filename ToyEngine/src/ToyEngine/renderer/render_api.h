#pragma once
#include "ToyEngine/enum.h"
#include "ToyEngine/renderer/vertex_array.h"

namespace ToyEngine
{
  class RenderAPI
  {
  public:
    // Enforce static class
    // Delete default, copy, and move constructors
    RenderAPI() {}
    virtual ~RenderAPI() {}

    static void Init(eRenderAPI api);
    static Scope<RenderAPI> s_instance;

    // Methods
    virtual void ClearSetBackground(const glm::vec4& clear_color) const = 0;
    virtual void BindTexture(eSamplerType texture_type, uint32_t texture_id) const = 0;
    virtual void DrawElements(ePrimType type, const VertexArray* vao) const = 0;
    virtual void PolygonMode(uint32_t face, uint32_t mode) const = 0;
    virtual void Enable(eParamType cap) const = 0;
    virtual void Disable(eParamType cap) const = 0;
    virtual void GetBooleanv(eParamType pname, bool* data) const = 0;
    virtual void GetIntegerv(eParamType pname, int* data) const = 0;
    virtual void DepthMask(bool flag) const = 0; 
    virtual void DepthFunc(eDepthFunc func) const = 0;
    virtual void CullFace(eParamType face_mode) const = 0;
    virtual void FrontFace(eParamType winding_order) const = 0;
  };

  class RenderCommand
  {
  public:
    // Enforce static class
    // Delete default, copy, and move constructors
    RenderCommand() = delete;
    RenderCommand(const RenderAPI&) = delete;
    RenderCommand(RenderAPI&&) = delete;

    // Delete the assignment operators
    RenderCommand& operator=(const RenderAPI&) = delete;
    RenderCommand& operator=(RenderAPI&&) = delete;

    static void ClearSetBackground(const glm::vec4& clear_color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f)) 
    {
      RenderAPI::s_instance->ClearSetBackground(clear_color); 
    }

    static void BindTexture(eSamplerType texture_type, uint32_t texture_id)
    {
      RenderAPI::s_instance->BindTexture(texture_type, texture_id);
    }
    
    static void DrawElements(ePrimType type, const VertexArray* vao)
    {
      RenderAPI::s_instance->DrawElements(type, vao);
    }
    
    static void PolygonMode(uint32_t face, uint32_t mode) 
    {
      RenderAPI::s_instance->PolygonMode(face, mode); 
    }

    static void Enable(eParamType cap)
    {
      RenderAPI::s_instance->Enable(cap);
    }

    static void Disable(eParamType cap)
    {
      RenderAPI::s_instance->Disable(cap);
    }

    static void GetBooleanv(eParamType pname, bool* data)
    {
      RenderAPI::s_instance->GetBooleanv(pname, data);
    }

    static void GetIntegerv(eParamType pname, int* data)
    {
      RenderAPI::s_instance->GetIntegerv(pname, data);
    }

    static void DepthMask(bool flag)
    {
      RenderAPI::s_instance->DepthMask(flag);
    }

    static void DepthFunc(eDepthFunc func)
    {
      RenderAPI::s_instance->DepthFunc(func);
    }

    static void CullFace(eParamType face_mode)
    {
      RenderAPI::s_instance->CullFace(face_mode);
    }

    static void FrontFace(eParamType winding_order)
    {
      RenderAPI::s_instance->FrontFace(winding_order);
    }

  };
}