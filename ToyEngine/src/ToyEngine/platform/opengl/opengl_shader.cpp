#include "pch.h"
#include "opengl_shader.h"
#include "ToyEngine/file_manager.h"
#include <glad/glad.h>

namespace ToyEngine
{
  OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertex_path, const std::string& fragment_path) :
    Shader(name)
  {
    // read shader from file
    std::string vertex_shader_code = FileManager::ReadSourceFile(vertex_path);
    std::string fragment_shader_code = FileManager::ReadSourceFile(fragment_path);

    // compile the shaders
    CompileShaderProgram(vertex_shader_code.c_str(), fragment_shader_code.c_str(), id_);
  }

  void OpenGLShader::Use()
  {
    if (id_ == 0) {
      TY_CORE_ERROR("SHADER::{} - Attempting to use invalid shader program (ID = 0)", name_);
      return;
    }

    glUseProgram(id_);
    
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
      TY_CORE_ERROR("SHADER::{}(id={}) - glUseProgram failed with error: {}", name_, id_, error);
    }
  }

  void ToyEngine::OpenGLShader::BindUniformBlock(const std::string& uniform_block, uint32_t binding_point) const
  {
    // Get the index of the "Matrices" uniform block in each shader program.
    // This index is used to refer to the block within the shader.
    
    //glGetUniformBlockIndex(id_, uniform_block);

    // Bind each uniform block index to a binding point (here, binding point 0).
    // This tells OpenGL that the "Matrices" block in each shader will use binding point 0.
    glUniformBlockBinding(id_, glGetUniformBlockIndex(id_, uniform_block.c_str()), binding_point);
  }

  void OpenGLShader::SetBool(const std::string& name, bool value) const
  {
    glUniform1i(glGetUniformLocation(id_, name.c_str()), (int)value);
  }
  void OpenGLShader::SetInt(const std::string& name, int value) const
  {
    glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
  }
  void OpenGLShader::SetFloat(const std::string& name, float value) const
  {
    glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
  }

  void OpenGLShader::SetFloat2(const std::string& name, float v0, float v1) const
  {
    glUniform2f(glGetUniformLocation(id_, name.c_str()), v0, v1);
  }

  void OpenGLShader::SetFloat3(const std::string& name, float v0, float v1, float v2) const
  {
    glUniform3f(glGetUniformLocation(id_, name.c_str()), v0, v1, v2);
  }

  void OpenGLShader::SetFloat4(const std::string& name, float v1, float v2, float v3, float v4) const
  {
    glUniform4f(glGetUniformLocation(id_, name.c_str()), v1, v2, v3, v4);
  }

  void OpenGLShader::SetMat4(const std::string& name, glm::mat4 value) const
  {
    glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
  }

  void OpenGLShader::CompileShaderProgram(const char* vertex_source, const char* fragement_source, uint32_t& shader_program)
  {
    int success;
    char info_log[512];

    // compile vertex shader
    uint32_t vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
      TY_CORE_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}", info_log);
    }

    // compile fragment shader
    uint32_t fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragement_source, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
      TY_CORE_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n{}", info_log);
    }

    // Create shader program
    shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader_program, 512, NULL, info_log);
      TY_CORE_ERROR("ERROR::SHADERPROGRAM::COMPILIATION_FAILED\n{}", info_log);
      
      glDeleteProgram(shader_program);
      shader_program = 0;
    }

    // Clean-up. Delete shaders. 
    // They are linked into our program now and no longer necessary.
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
  }
}