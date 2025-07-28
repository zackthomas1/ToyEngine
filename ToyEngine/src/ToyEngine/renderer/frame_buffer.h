#pragma once

namespace ToyEngine
{
	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() {};

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void CopyColorBuffer() const = 0;
		virtual void CopyDepthBuffer() const = 0;
		virtual void CopyStencilBuffer() const = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void Status() const = 0;

		virtual uint32_t GetColorAttachment() const = 0;
		uint32_t GetWidth() const { return width_; }
		uint32_t GetHeight() const { return height_; }

		static Ref<FrameBuffer> Create(uint32_t width, uint32_t height);
	protected:
		FrameBuffer(uint32_t width, uint32_t height) : width_(width), height_(height) {}
		uint32_t width_, height_;
	};
}