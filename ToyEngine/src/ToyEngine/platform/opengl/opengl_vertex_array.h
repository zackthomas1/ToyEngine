#pragma once
#include "ToyEngine/renderer/vertex_array.h"

namespace ToyEngine 
{

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void AddBuffer(const Ref<VertexBuffer> buffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer> buffer) override;

	private:
		uint32_t id_;
		uint32_t vertex_buffer_index_ = 0; 
		Vector<Ref<VertexBuffer>> vertex_buffers_; 
		Ref<IndexBuffer> index_buffer_;
	};
}