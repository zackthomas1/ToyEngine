#pragma once
#include "ToyEngine/renderer/frame_buffer.h"

namespace ToyEngine {
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(uint32_t width, uint32_t height);
		~OpenGLFrameBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual uint32_t GetColorAttachment() const override;
		virtual uint32_t GetDepthAttachment() const override;

	private:
		uint32_t color_attachment_id_, depth_attachment_id_;
	};
}