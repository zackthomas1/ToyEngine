#include "pch.h"
#include "opengl_vertex_array.h"
#include <glad/glad.h>

namespace ToyEngine
{

	static GLenum ShaderDataTypeToOpenGL(eShaderDataType type)
	{
		switch (type)
		{
			case eShaderDataType::Int	:	return GL_INT;
			case eShaderDataType::Bool	:	return GL_BOOL;
			case eShaderDataType::Float	:	return GL_FLOAT;
			case eShaderDataType::Vec2	:	return GL_FLOAT;
			case eShaderDataType::Vec3	:	return GL_FLOAT;
			case eShaderDataType::Vec4	:	return GL_FLOAT;
			case eShaderDataType::Mat3	:	return GL_FLOAT;
			case eShaderDataType::Mat4	:	return GL_FLOAT;
		}
		TY_CORE_ASSERT(false, "Unknown ShaderDatayType.");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
		: vertex_buffer_index_(0)
	{
		glGenVertexArrays(1, &id_);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			TY_CORE_ERROR("OpenGL Error after glGenVertexArrays: {}", error);
		}
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &id_);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(id_); 
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddBuffer(const Ref<VertexBuffer> buffer)
	{
		TY_CORE_ASSERT(buffer->GetLayout().GetElements().size(), "Vertex Buffer missing layout"); 

		glBindVertexArray(id_); 
		buffer->Bind();

		const auto& layout = buffer->GetLayout(); 
		for (const auto& element : layout) 
		{
			glEnableVertexAttribArray(vertex_buffer_index_); 
			glVertexAttribPointer(vertex_buffer_index_,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGL(element.m_type),
				element.m_normalize ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.m_offset
			);
			++vertex_buffer_index_;
		}
		vertex_buffers_.push_back(buffer);
		
		// Unbind vertex buffer and vao to clean up state
		buffer->Unbind();
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer> buffer)
	{
		glBindVertexArray(id_);
		buffer->Bind(); 
		index_buffer_ = buffer;

		// Unbind vao to clean up state
		glBindVertexArray(0);
	}
}