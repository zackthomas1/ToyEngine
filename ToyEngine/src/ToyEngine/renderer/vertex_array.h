#pragma once
#include "ToyEngine/renderer/buffer.h"

namespace ToyEngine {
	class VertexArray
	{
	public:

		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void AddBuffer(const Ref<VertexBuffer> buffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer> buffer) = 0;
		virtual const Ref<IndexBuffer> GetIndexBuffer() const = 0;

		static Ref<VertexArray> Create();
	protected:
		VertexArray(){}
	private:

	};
}