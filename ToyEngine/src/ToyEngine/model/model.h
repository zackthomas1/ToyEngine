#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ToyEngine/entity.h"
#include "ToyEngine/model/mesh.h"
#include "ToyEngine/model/material.h"

namespace ToyEngine
{
  class Model : public Entity
  {
  public:
    Model(const char* path, bool flip_vertically = false, bool gamma = false);

    virtual void Update(const TimeStep& time_step) override;
    virtual void Render(const glm::mat4& world_transform = glm::mat4(1.0f)) const override;

    static Ref<Model> Create(const char* path, bool flip_vertically = false, bool gamma = false);
  public:
    Ref<Shader> m_shader;
    Vector<Ref<Mesh>> m_meshes;
    Vector<Ref<Texture2D>> m_textures;
  private:
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Ref<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
    void loadMaterialTextures(aiMaterial* mat, aiTextureType type, Vector<Ref<Texture2D>> &textures);
  private:
    std::string directory_;
    bool flip_vertically_, gamma_;
  };
}