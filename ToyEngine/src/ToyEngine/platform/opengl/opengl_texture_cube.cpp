#include "pch.h"
#include "opengl_texture_cube.h"
#include <glad/glad.h>
#include "ToyEngine/stb_image.cpp"

namespace ToyEngine
{
	OpenGLTextureCube::OpenGLTextureCube(const Array<std::string, 6>& face_paths, bool flip_vertically)
		: TextureCube(face_paths)
	{
		// Generate a new OpenGL texture object and bind it to the GL_TEXTURE_CUBE_MAP target.
		glGenTextures(1, &id_);
		glBindTexture(GL_TEXTURE_CUBE_MAP, id_);

		// Set cube map texture wrapping parameters:
		// For cube maps, we typically use GL_CLAMP_TO_EDGE to avoid seams between faces
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		// Set texture filtering parameters:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Load the 6 faces of the cube map
		// Order: +X, -X, +Y, -Y, +Z, -Z (right, left, top, bottom, front, back)
		stbi_set_flip_vertically_on_load(flip_vertically);

		bool all_faces_loaded = true;
		int width, height, nr_channels;

		for (uint32_t i = 0; i < 6; i++)
		{
			// Load image data from file using stb_image
			stbi_uc* data = stbi_load(m_face_paths[i].c_str(), &width, &height, &nr_channels, 0);
			
			if (data)
			{
				// Store dimensions from first face (all faces should be same size)
				if (i == 0)
				{
					width_ = width;
					height_ = height;
					nr_channels_ = nr_channels;
				}

				// Determine OpenGL format based on number of channels
				GLenum internal_format = 0, data_format = 0;
				if (nr_channels == 3) {
					internal_format = GL_RGB8;
					data_format = GL_RGB;
				}
				else if (nr_channels == 4) {
					internal_format = GL_RGBA8;
					data_format = GL_RGBA;
				}

				// Upload the image data to the appropriate cube map face
				// GL_TEXTURE_CUBE_MAP_POSITIVE_X + i gives us each face in order
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, data_format, width, height, 0, data_format, GL_UNSIGNED_BYTE, data);
				
				// Free the image data
				stbi_image_free(data);
			}
			else
			{
				TY_CORE_ERROR("Failed to load cube map face: {0}", m_face_paths[i]);
				all_faces_loaded = false;
				// Continue loading other faces even if one fails
			}
		}

		// Generate mipmaps for the cube map if all faces were loaded successfully
		if (all_faces_loaded)
		{
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}
	}

	OpenGLTextureCube::OpenGLTextureCube(const std::string& path, bool flip_vertically)
		: TextureCube(path)
	{
		// This constructor is for loading a single cubemap file
		// Implementation depends on the specific format (e.g., HDR, DDS, etc.)
		// For now, we'll create a placeholder implementation
		TY_CORE_ERROR("Single file cube map loading not yet implemented");
		
		// Generate a placeholder texture to avoid crashes
		glGenTextures(1, &id_);
		glBindTexture(GL_TEXTURE_CUBE_MAP, id_);
		
		// Set basic parameters
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		// Create empty faces for now
		width_ = 1;
		height_ = 1;
		nr_channels_ = 3;
		
		unsigned char white_pixel[3] = {255, 255, 255};
		for (uint32_t i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, white_pixel);
		}
	}

	OpenGLTextureCube::~OpenGLTextureCube()
	{
		glDeleteTextures(1, &id_);
	}

	void OpenGLTextureCube::Bind(uint32_t texture_unit) const
	{
		// Bind the cube map texture to the specified texture unit
		glActiveTexture(GL_TEXTURE0 + texture_unit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, id_);
	}
}