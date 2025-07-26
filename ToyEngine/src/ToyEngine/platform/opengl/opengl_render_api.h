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
		virtual void GenVertexArrays(uint32_t size, uint32_t& vao) const override;
		virtual void GenBuffers(uint32_t size, uint32_t & buffer_object) const override;
		virtual void BindVertexArray(uint32_t vao) const override;
		virtual void BindBuffer(eBufferType buffer_type, uint32_t buffer_object) const override;
		virtual void BufferData(eBufferType buffer_type, uint32_t size, const void* data) const override;
		virtual void DeleteVertexArray(uint32_t& id) const override;
		virtual void EnableVertexAttribArray(uint32_t index) const override;
		virtual void VertexAttribPointer(uint32_t location, uint32_t elements, eDataType data_type, uint32_t size, uint32_t offset) const override;
		virtual void DeleteBuffer(uint32_t& id) const override;
		virtual void DrawArrays(ePrimType type, uint32_t start_index, uint32_t vertices) const override;
		virtual void DrawElements(ePrimType type, uint32_t indices, uint32_t offset) const override;
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