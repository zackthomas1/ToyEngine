#include "pch.h"
#include "opengl_texture_2d.h"
#include <glad/glad.h>
#include "stb_image.h"

namespace ToyEngine
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string &path, eTextureType type, bool flip_vertically)
	: Texture2D(path, type)
	{
		// Generate a new OpenGL texture object and bind it to the GL_TEXTURE_2D target.
		// This creates a handle (id_) for the texture and makes it the current 2D texture.
		glGenTextures(1, &id_);
		glBindTexture(GL_TEXTURE_2D, id_);

		// Set texture wrapping parameters:
		// GL_TEXTURE_WRAP_S: wrapping mode on the S (X) axis (e.g., GL_REPEAT, GL_CLAMP_TO_EDGE)
		// GL_TEXTURE_WRAP_T: wrapping mode on the T (Y) axis
		uint32_t wrap_s = GL_CLAMP_TO_EDGE, wrap_t = GL_CLAMP_TO_EDGE;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);

		// Set texture filtering parameters:
		// GL_TEXTURE_MIN_FILTER: filtering when texture is minified (e.g., GL_LINEAR, GL_NEAREST, mipmap options)
		// GL_TEXTURE_MAG_FILTER: filtering when texture is magnified
		uint32_t min_filter = GL_LINEAR_MIPMAP_LINEAR, mag_filter = GL_LINEAR;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);

		// --- stb_image: Load image data from file ---
		// Set whether to vertically flip the image on load (OpenGL expects 0,0 at bottom left).
		int width, height, nr_channels;
		stbi_set_flip_vertically_on_load(flip_vertically);
		// Load image data from file using stb_image.
		// Returns a pointer to the pixel data, and fills width, height, and nr_channels.
		stbi_uc* data = stbi_load(m_path.c_str(), &width, &height, &nr_channels, 0);
		width_ = width;
		height_ = height;
		nr_channels_ = nr_channels;

		// Determine OpenGL internal format and data format based on number of channels.
		GLenum internal_format = 0, data_format = 0;
		if (nr_channels_ == 3) {
			internal_format = GL_RGB8;
			data_format = GL_RGB;
		}
		else if (nr_channels_ == 4) {
			internal_format = GL_RGBA8;
			data_format = GL_RGBA;
		}

		// --- Send image data to GPU ---
		if (data){
			// Upload the image data to the currently bound OpenGL texture.
			// glTexImage2D copies the pixel data from CPU memory to GPU memory.
			glTexImage2D(GL_TEXTURE_2D, 0, data_format, width_, height_, 0, data_format, GL_UNSIGNED_BYTE, data);
			// Generate mipmaps for the texture for better minification filtering.
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			TY_CORE_ERROR("Failed to load texture");
		}
		// Free the image data loaded by stb_image from CPU memory.
		stbi_set_flip_vertically_on_load(false);
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &id_);
	}

	void OpenGLTexture2D::Bind(uint32_t texture_unit) const
	{
		// In OpenGL, textures must be bound to a texture unit before use in shaders.
		// glActiveTexture selects the active texture unit (e.g., GL_TEXTURE0 + texture_unit).
		// glBindTexture binds the texture object to the GL_TEXTURE_2D target for the selected unit.
		glActiveTexture(GL_TEXTURE0 + texture_unit);
		glBindTexture(GL_TEXTURE_2D, id_);

		// Alternatively: glBindTextureUnit combines functionality of glActiveTexture and glBindTexture into a single call
		// Example: glBindTextureUnit(texture_unit, id_)
	}
}