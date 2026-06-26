#include "pch.h"
#include "opengl_texture_cube.h"
#include <glad/glad.h>
#include "stb_image.h"

namespace ToyEngine {
  OpenGLTextureCube::OpenGLTextureCube(const std::initializer_list<std::string>& files)
    : width_(0), height_(0), nrChannels_(0)
  {
    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id_);

    int first_width = 0, first_height = 0, first_channels = 0;
    for (uint32_t i = 0; i < files.size(); i++) {
      const char* file_name = (files.begin() + i)->c_str();
      unsigned char* data = stbi_load(file_name, &width_, &height_, &nrChannels_, 0);
      
      if (data) {
        // Store dimensions from the first face
        if (i == 0) {
          first_width = width_;
          first_height = height_;
          first_channels = nrChannels_;
        }
        else if (width_ != first_width || height_ != first_height || nrChannels_ != first_channels) {
          TY_CORE_ERROR("Cubemap face {} has different dimensions: ({},{}) vs ({},{})",
            i, width_, height_, first_width, first_height);
          stbi_image_free(data);
          break;
        }

        TY_CORE_INFO("Loaded cubemap face {}: {}x{}, {} channels", i, width_, height_, nrChannels_);

        GLenum format = GL_RGB;
        if (nrChannels_ == 1)
          format = GL_RED;
        else if (nrChannels_ == 3)
          format = GL_RGB;
        else if (nrChannels_ == 4)
          format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width_, height_, 0, format, GL_UNSIGNED_BYTE, data);
        
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
          TY_CORE_ERROR("OpenGL error uploading cubemap face {}: {}", i, error);
        }
      }
      else {
        TY_CORE_ERROR("Cubemap tex failed to load at path {}", file_name);
      }
      stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Check if cubemap is complete
    if (glGetError() != GL_NO_ERROR) {
      TY_CORE_ERROR("OpenGL error after cubemap creation");
    }

    GenerateMipMaps();

    // Unbind the texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  }

  void OpenGLTextureCube::Bind(uint32_t slot) const
  {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id_);
  }

  void OpenGLTextureCube::GenerateMipMaps()
  {
    //bind cubemap texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, id_); 

    // enable mipmap generation
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // generate mipmaps
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP); 

    //calculate max mip level
    int size = std::max(width_, height_);
    max_mip_level_ = static_cast<int>(std::floor(std::log2(size)));

    // Check if error after mipmap generation
    if (glGetError() != GL_NO_ERROR) {
      TY_CORE_ERROR("OpenGL error after generating mipmaps.");
    }

    // release binding
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  }
}