#include "pch.h"
#include "opengl_uniform_buffer.h"

#include <glad/glad.h>

namespace ToyEngine
{
    OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding)
        : m_size(size)
    {
        // Generate and bind the uniform buffer object
        glGenBuffers(1, &m_buffer_id);
        glBindBuffer(GL_UNIFORM_BUFFER, m_buffer_id);
        glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
        
        // Bind buffer to binding point
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_buffer_id);
        
        // Unbind the buffer
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        
        TY_CORE_INFO("Created OpenGL Uniform Buffer with ID: {}, Size: {}, Binding: {}", m_buffer_id, size, binding);
    }

    OpenGLUniformBuffer::~OpenGLUniformBuffer()
    {
        TY_CORE_INFO("Destroying OpenGL Uniform Buffer with ID: {}", m_buffer_id);
        glDeleteBuffers(1, &m_buffer_id);
    }

    void OpenGLUniformBuffer::Bind(uint32_t binding)
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_buffer_id);
    }

    void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_buffer_id);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}