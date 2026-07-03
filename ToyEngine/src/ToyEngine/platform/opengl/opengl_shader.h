#pragma once
#include "ToyEngine/renderer/shader.h"

namespace ToyEngine
{
  class OpenGLShader : public Shader
  {
  public:
    // constructor reads and builds the shader
    OpenGLShader(const std::string& name, const std::string& vertex_path, const std::string& fragment_path);

    // use/activate the shader 
    virtual void Use() override;
    virtual void BindUniformBlock(const std::string& uniform_block, uint32_t binding_point) const override;

    // utility uniform functions
    virtual void SetBool(const std::string& name, bool value) const override;
    virtual void SetInt(const std::string& name, int value) const override;
    virtual void SetFloat(const std::string& name, float value) const override;
    virtual void SetFloat2(const std::string& name, float v0, float v1) const override;
    virtual void SetFloat3(const std::string& name, float v0, float v1, float v2) const override;
    virtual void SetFloat4(const std::string& name, float v1, float v2, float v3, float v4) const override;
    virtual void SetMat4(const std::string& name, glm::mat4 value) const override;

  private:
    /// @brief Compiles the vertex and fragment shader source code into a shader program.
    /// @param vertex_source The source code of the vertex shader.
    /// @param fragment_source The source code of the fragment shader.
    /// @param shader_program A reference to the variable that will hold the compiled shader program ID.
    void CompileShaderProgram(const char* vertex_source, const char* fragement_source, uint32_t& shader_program);
  };
}