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
		virtual void GenVertexArrays(const int size, uint32_t& vao) const override;
		virtual void GenBuffers(const int size, uint32_t & buffer_object) const override;
		virtual void BindVertexArray(const uint32_t& vao) const override;
		virtual void BindBuffer(eBufferType buffer_type, const uint32_t buffer_object) const override;
		virtual void BufferData(eBufferType buffer_type, uint32_t size, const void* data) const override;
		virtual void DeleteVertexArray(uint32_t& id) const override;
		virtual void CreateVertexAttrib(uint32_t location, uint32_t elements, eDataType data_type, uint32_t size, uint32_t offset) const override;
		virtual void DeleteBuffer(uint32_t& id) const override;
		virtual void DrawArrays(uint32_t vertices) const override;
		virtual void DrawIndexed(uint32_t vao) const override;
		virtual void PolygonMode(uint32_t face, uint32_t mode) const override;
	};
}