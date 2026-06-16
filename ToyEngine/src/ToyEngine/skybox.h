#pragma once
#include "ToyEngine/entity.h"
#include "ToyEngine/renderer/shader.h"
#include "ToyEngine/renderer/vertex_array.h"
#include "ToyEngine/renderer/texture_cube.h"

namespace ToyEngine {
  class Skybox : public Entity
  {
  public:
    Skybox(Ref<TextureCube> texture, Ref<Shader> shader);
    ~Skybox();

    virtual void Render(const glm::mat4& world_transform = glm::mat4(1.0f)) const override;
    bool IsValid() const { return m_vao && texture_ && m_shader; }

    Ref<Shader> m_shader;
    Ref<VertexArray> m_vao;
  private:
    Ref<TextureCube> texture_;
  };
}
