
#include "pch.h"
#include "opengl_buffer.h"
#include <glad/glad.h>

namespace ToyEngine {

	// Vertex Buffer
	//---------------
	OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, uint32_t size)
	{
		if (!data || size == 0) 
		{
			TY_CORE_ERROR("Invalid vertex buffer data or size");
			return;
		}

		glGenBuffers(1, &id_);
		glBindBuffer(GL_ARRAY_BUFFER, id_);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

		// Add OpenGL error checking
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			TY_CORE_ERROR("OpenGL Error in VertexBuffer creation: {}", error);
			glDeleteBuffers(1, &id_);
			id_ = 0;
		}
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		if (id_ != 0) {
			glDeleteBuffers(1, &id_);
			id_ = 0;
		}
	}
	void OpenGLVertexBuffer::Bind() const
	{
		if (id_ == 0) {
			TY_CORE_ERROR("Attempting to bind invalid vertex buffer");
			return;
		}
	
		glBindBuffer(GL_ARRAY_BUFFER, id_);
	}
	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// Index Buffer
	//---------------
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: count_(count)
	{
		if (!indices || count == 0) 
		{
			TY_CORE_ERROR("Invalid index buffer data or count");
			return;
		}
	
		glGenBuffers(1, &id_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);

		// Add OpenGL error checking
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			TY_CORE_ERROR("OpenGL Error in IndexBuffer creation: {}", error);
			glDeleteBuffers(1, &id_);
			id_ = 0;
		}
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		if (id_ != 0) {
			glDeleteBuffers(1, &id_);
			id_ = 0; 
		}
	}

	void ToyEngine::OpenGLIndexBuffer::Bind() const
	{
		if (id_ == 0) {
			TY_CORE_ERROR("Attempting to bind invalid index buffer");
			return;
		}
	
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
	}
	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}