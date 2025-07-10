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
		virtual void BindTexture2D() const override {}
		virtual void CreateVertexArray(uint32_t& vao) const override;
		virtual void BindVertexArray(const uint32_t& vao) const override;
		virtual void DeleteVertexArray(uint32_t& id) const override;
		virtual void CreateVertexBuffer(uint32_t& vbo, float* vertex_array, const int size) const override;
		virtual void CreateVertexAttrib(uint32_t& vao) const override {}
		virtual void DeleteBuffer(uint32_t& id) const override;
		virtual void DrawArrays(uint32_t vertices) const override;
		virtual void DrawIndexed(uint32_t vao) const override;
		virtual void PolygonMode(uint32_t face, uint32_t mode) const override;
	};
}