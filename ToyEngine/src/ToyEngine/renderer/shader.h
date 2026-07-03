#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ToyEngine
{
  class Shader
  {
  public:
    virtual ~Shader() {}

    virtual void Use() = 0;
    virtual void BindUniformBlock(const std::string& uniform_block, uint32_t binding_point) const = 0;

    virtual void SetBool(const std::string& name, bool value) const = 0;
    virtual void SetInt(const std::string& name, int value) const = 0;
    virtual void SetFloat(const std::string& name, float value) const = 0;
    virtual void SetFloat2(const std::string& name, float v0, float v1) const = 0;
    virtual void SetFloat3(const std::string& name, float v0, float v1, float v2) const = 0;
    virtual void SetFloat4(const std::string& name, float v1, float v2, float v3, float v4) const = 0;
    virtual void SetMat4(const std::string& name, glm::mat4 value) const = 0;

    /// @brief Returns the unique identifier of the shader program.
    /// @return The unique identifier of the shader program.
    uint32_t id() const { return id_; }
    const std::string& GetName() const { return name_; }

    static Ref<Shader> Create(const char* shader_name, const char* vertex_path, const char* fragment_path);
  protected:
    // constructor reads and builds the shader
    Shader(const std::string& name) : name_(name) {}
    uint32_t id_;
    std::string name_;
  };
}