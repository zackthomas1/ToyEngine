#pragma once
#include "ToyEngine/renderer/buffer.h"

namespace ToyEngine
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* data, uint32_t size);
		~OpenGLVertexBuffer ();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual const BufferLayout& GetLayout() const override { return layout_; }
		virtual void SetLayout(const BufferLayout& layout) override { layout_ = layout; }

	private:
		uint32_t id_;
		BufferLayout layout_;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override { return count_; }
	private: 
		uint32_t id_, count_;
	};

}