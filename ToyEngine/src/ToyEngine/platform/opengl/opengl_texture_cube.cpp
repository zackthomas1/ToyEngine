#include "pch.h"
#include "opengl_texture_cube.h"
#include <glad/glad.h>
#include "stb_image.h"

namespace ToyEngine {
	OpenGLTextureCube::OpenGLTextureCube(Array<std::string, CUBE_SIDES>& files)
	{
		glGenTextures(1, &id_);
		glBindTexture(GL_TEXTURE_CUBE_MAP, id_);

		int first_width = 0, first_height = 0, first_channels = 0;
		for (uint32_t i = 0; i < files.size(); i++) {
			int width, height, nrChannels;
			unsigned char* data = stbi_load(files[i].c_str(), &width, &height, &nrChannels, 0);
			
			if (data) {
				// Store dimensions from the first face
				if (i == 0) {
					first_width = width;
					first_height = height;
					first_channels = nrChannels;
				}
				else if (width != first_width || height != first_height || nrChannels != first_channels) {
					TY_CORE_ERROR("Cubemap face {} has different dimensions: ({},{}) vs ({},{})",
						i, width, height, first_width, first_height);
					stbi_image_free(data);
					break;
				}

				TY_CORE_INFO("Loaded cubemap face {}: {}x{}, {} channels", i, width, height, nrChannels);

				GLenum format = GL_RGB;
				if (nrChannels == 1)
					format = GL_RED;
				else if (nrChannels == 3)
					format = GL_RGB;
				else if (nrChannels == 4)
					format = GL_RGBA;

				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				
				GLenum error = glGetError();
				if (error != GL_NO_ERROR) {
					TY_CORE_ERROR("OpenGL error uploading cubemap face {}: {}", i, error);
				}
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

		// Check if cubemap is complete
		if (glGetError() != GL_NO_ERROR) {
			TY_CORE_ERROR("OpenGL error after cubemap creation");
		}

		// Unbind the texture
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void OpenGLTextureCube::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, id_);
	}
}