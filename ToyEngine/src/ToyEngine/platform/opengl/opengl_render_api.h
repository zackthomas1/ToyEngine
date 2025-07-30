#pragma once
#include "ToyEngine/renderer/render_api.h"

namespace ToyEngine
{
	class OpenGLRenderAPI : public RenderAPI
	{
	public:
		OpenGLRenderAPI() {}
		~OpenGLRenderAPI() {}

		// Methods
		virtual void ClearSetBackground(const glm::vec4& clear_color) const override;
		virtual void BindTexture(eSamplerType texture_type, uint32_t texture_id) const;
		virtual void DrawElements(ePrimType type, const VertexArray* vao) const override;
		virtual void PolygonMode(uint32_t face, uint32_t mode) const override;
		virtual void Enable(eParamType cap) const override;
		virtual void Disable(eParamType cap) const override;
		virtual void GetBooleanv(eParamType pname, bool* data) const override;
		virtual void GetIntegerv(eParamType pname, int* data) const override;
		virtual void DepthMask(bool flag) const override;
		virtual void DepthFunc(eDepthFunc func) const override;
		virtual void CullFace(eParamType face_mode) const override;
		virtual void FrontFace(eParamType winding_order) const override;
	};
}