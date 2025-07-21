#include "pch.h"
#include "opengl_frame_buffer.h"
#include <glad/glad.h>
#include "ToyEngine/log.h"

namespace ToyEngine
{
	OpenGLFrameBuffer::OpenGLFrameBuffer(uint32_t width, uint32_t height) : FrameBuffer(width, height)
	{
		// Generate and bind the framebuffer
		glGenFramebuffers(1, &id_);
		glBindFramebuffer(GL_FRAMEBUFFER, id_);

		// Create color attachment texture
		glGenTextures(1, &color_attachment_id_);
		glBindTexture(GL_TEXTURE_2D, color_attachment_id_);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attachment_id_, 0);

		// Create depth attachment texture
		glGenTextures(1, &depth_attachment_id_);
		glBindTexture(GL_TEXTURE_2D, depth_attachment_id_);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width_, height_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_attachment_id_, 0);

		// Check framebuffer completeness
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			TY_CORE_ERROR("Framebuffer is not complete!");
		}

		// Unbind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &id_);
		glDeleteTextures(1, &color_attachment_id_);
		glDeleteTextures(1, &depth_attachment_id_);
	}

	void OpenGLFrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id_);
		glViewport(0, 0, width_, height_);
	}

	void OpenGLFrameBuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	uint32_t OpenGLFrameBuffer::GetColorAttachment() const
	{
		return color_attachment_id_;
	}

	uint32_t OpenGLFrameBuffer::GetDepthAttachment() const
	{
		return depth_attachment_id_;
	}
}