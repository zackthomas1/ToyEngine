#include "pch.h"
#include "opengl_texture_cube.h"
#include <glad/glad.h>
#include "ToyEngine/stb_image.cpp"

namespace ToyEngine {
    OpenGLTextureCube::OpenGLTextureCube(Array<std::string, CUBE_SIDES>& files)
    {
        glGenTextures(1, &id_);
        glBindTexture(GL_TEXTURE_CUBE_MAP, id_);

        for (uint32_t i = 0; i < files.size(); i++) {
            unsigned char* data = stbi_load(files[i].c_str(), &width_, &height_, &nrChannels_, 0);
            if (data) {
                TY_CORE_INFO("Loaded cubemap face {}: {}x{}, {} channels", i, width_, height_, nrChannels_);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }
            else {
                TY_CORE_ERROR("Cubemap tex failed to load at path {}", files[i]);
            }
            stbi_image_free(data);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    void OpenGLTextureCube::Bind() const
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, id_);
    }
}