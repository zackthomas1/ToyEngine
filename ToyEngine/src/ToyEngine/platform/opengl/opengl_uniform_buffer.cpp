#include "pch.h"
#include "opengl_uniform_buffer.h"
#include <glad/glad.h>

namespace ToyEngine
{
  OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding_point) : UniformBuffer(size, binding_point)
  {
    // Create a uniform buffer object (UBO) to store the data for the uniform block.
    glGenBuffers(1, &id_);
    glBindBuffer(GL_UNIFORM_BUFFER, id_);
    // Allocate space for two 4x4 matrices (view and projection) in the buffer.
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // Bind the buffer to the same binding point (0) used above.
    // This links the buffer's data to the "Matrices" block in all shaders using binding point 0.
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
    // The first glBufferSubData call uploads the camera's view matrix to the first half of the buffer.
    // The second glBufferSubData call uploads the camera's projection matrix to the second half of the buffer.
    // This ensures both matrices are available to all shaders using the "Matrices" uniform block.
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
