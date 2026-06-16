#pragma once
#include "ToyEngine/renderer/shader.h"
#include "ToyEngine/renderer/texture_2d.h"
#include "ToyEngine/renderer/texture_cube.h"

namespace ToyEngine
{
  class Material
  {
  public:
    Material() : 
      roughness_(32.0f), metallic_(0.0f), refractive_index_(1.0f)
    { TY_CORE_WARN("Material textures empty."); }
    Material(const Vector<Ref<Texture2D>>& textues) : 
      roughness_(32.0f), metallic_(0.0f), refractive_index_(1.0f), textures_(textues) 
    {}
    Material(const Vector<Ref<Texture2D>>& textues, Ref<TextureCube> environment_map) : 
      roughness_(32.0f), metallic_(0.0f), refractive_index_(1.0f), textures_(textues), environment_map_(environment_map)
    {}

    virtual ~Material() {}

    const Vector<Ref<Texture2D>>& getTextures() const { return textures_; }
    const Ref<TextureCube> getEnvironmentMap() const { return environment_map_; }

    void SetEnvironmentMap(Ref<TextureCube> environment_map) { environment_map_ = environment_map; }
    void SetRoughness(float roughness) { roughness_ = roughness; }
    void SetMetallic(float metallic) { metallic_ = metallic; }
    void SetTransmission(float transmission) { transmission_ = transmission; }
    void SetRefractiveIndex(float index) { refractive_index_ = index; }

    void Bind(Ref<Shader> shader);
  private:
    Ref<TextureCube> GetDefaultEnvironmentMap();
    float roughness_, metallic_, transmission_, refractive_index_;
    Vector<Ref<Texture2D>> textures_;
    Ref<TextureCube> environment_map_;
    static Ref<TextureCube> s_default_environment_map;
  };
}