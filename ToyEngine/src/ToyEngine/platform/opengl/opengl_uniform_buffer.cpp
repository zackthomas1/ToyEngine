#include "pch.h"
#include "opengl_uniform_buffer.h"
#include <glad/glad.h>

namespace ToyEngine
{
  OpenGLUniformBuffer::OpenGLUniformBuffer(const std::string& name, uint32_t size, uint32_t binding_point)
    : UniformBuffer(name, size, binding_point)
  {
    // Create a uniform buffer object (UBO) to store the data for the uniform block.
    glGenBuffers(1, &id_);
    glBindBuffer(GL_UNIFORM_BUFFER, id_);
    // Allocate space for buffer of requested size.
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // Bind the buffer to the same binding point.
    // This links the buffer's data to the block in all shaders using binding point.
    glBindBufferRange(GL_UNIFORM_BUFFER, binding_point_, id_, 0, size_);
  }

  OpenGLUniformBuffer::~OpenGLUniformBuffer()
  {
    glDeleteBuffers(1, &id_);
  }

  void OpenGLUniformBuffer::Bind() const
  {
    glBindBuffer(GL_UNIFORM_BUFFER, id_);
  }

  void OpenGLUniformBuffer::Unbind() const
  {
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  void ToyEngine::OpenGLUniformBuffer::SetData(uint32_t offset, uint32_t size, const void* data)
  {
    // Bind the uniform buffer object (UBO) to update its data.
    glBindBuffer(GL_UNIFORM_BUFFER, id_);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  void OpenGLUniformBuffer::BindToBindingPoint(uint32_t binding_point) const
  {
    // Expects a target, a binding point index and a uniform buffer object.
    glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, id_ );
    
    // Alternatively:  se glBindBufferRange which expects an extra offset and size parameter
    // This way you can bind only a specific range of the uniform buffer to a binding point.
    // Using glBindBufferRange multiple different uniform blocks linked to a single uniform buffer object.
    //glBindBufferRange(GL_UNIFORM_BUFFER, binding_point, id_, 0, size_);
  }
}