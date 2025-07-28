#include "pch.h"
#include "opengl_frame_buffer.h"
#include <glad/glad.h>

namespace ToyEngine 
{
	OpenGLFrameBuffer::OpenGLFrameBuffer(uint32_t width, uint32_t height) : 
	FrameBuffer(width, height)
	{
		TY_CORE_ASSERT(width > 0  && height > 0, "Render buffer width and height must be positive.");
		TY_CORE_ASSERT(width <= GL_MAX_RENDERBUFFER_SIZE && height <= GL_MAX_RENDERBUFFER_SIZE, "Frame buffer size expands OpenGL max render buffer size");
		
		CreateBuffers(width, height);
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		DeleteBuffers();
	}
	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id_);

	}
	void OpenGLFrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		DeleteBuffers(); 
		CreateBuffers(width, height);
	}

	void OpenGLFrameBuffer::Status() const
	{
	}

	void OpenGLFrameBuffer::CreateBuffers(uint32_t width, uint32_t height)
	{
		glGenFramebuffers(1, &id_);

		//It is also possible to bind a framebuffer to a read or write target specifically 
		// by binding to GL_READ_FRAMEBUFFER or GL_DRAW_FRAMEBUFFER respectively.
		glBindFramebuffer(GL_FRAMEBUFFER, id_);

		// Create a texture and bind to color attachment.
		glGenTextures(1, &color_attachment_id_);
		glBindTexture(GL_TEXTURE_2D, color_attachment_id_);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attachment_id_, 0);

		glBindTexture(GL_TEXTURE_2D, 0);

		// create render buffer object:  can not be directly read from.
		// store all render data directly in buffer without conversions to texture-specificc formats
		// faster as writeable storage, but can not directly read from them
		// often used as depth and stencil attachments
		glGenRenderbuffers(1, &depth_stencil_attatchment_id_);
		glBindRenderbuffer(GL_RENDERBUFFER, depth_stencil_attatchment_id_);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

		//attach render buffer object to framebuffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_stencil_attatchment_id_);

		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		// the general rule is that if you never need to sample data from a specific buffer, 
		// it is wise to use a renderbuffer object for that specific buffer.If you need to sample data 
		// from a specific buffer like colors or depth values, you should use a texture attachment


		//For a framebuffer to be complete the following requirements have to be satisfied :
		// 1. We have to attach at least one buffer(color, depth or stencil buffer).
		// 2. There should be at least one color attachment.
		// 3. All attachments should be complete as well(reserved memory).
		// 4. Each buffer should have the same number of samples.
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			TY_CORE_ERROR("FRAMEBUFFER: Framebuffer is not complete");
		}
		//Bind the default framebuffer for rendering operations to have a visual impact
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::DeleteBuffers()
	{
		if (id_) {
			glDeleteFramebuffers(1, &id_);
			id_ = 0;
		}
		if (color_attachment_id_) {
			glDeleteTextures(1, &color_attachment_id_);
			color_attachment_id_ = 0;
		}
		if (depth_stencil_attatchment_id_) {
			glDeleteRenderbuffers(1, &depth_stencil_attatchment_id_);
			depth_stencil_attatchment_id_ = 0;
		}
	}
}
