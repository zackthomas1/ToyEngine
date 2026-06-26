#pragma once
#include "ToyEngine/renderer/texture_2d.h"

namespace ToyEngine
{
  class OpenGLTexture2D : public Texture2D
  {
  public:
    OpenGLTexture2D(const std::string &path, eTextureType type, bool flip_vertically = false);
    ~OpenGLTexture2D();

    virtual void Bind(uint32_t texture_unit) const override;
  };
}