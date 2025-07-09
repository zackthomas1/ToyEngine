#include "pch.h"
#include "texture_2d.h"
#include <glad/glad.h>
#include "stb_image.h"

namespace ToyEngine
{
	Texture2D::Texture2D(std::string &path, eTextureType type, bool flip_vertically)
		: m_path(path), m_type(type)
	{
		// generate texture sampler
		glGenTextures(1, &id_);
		glBindTexture(GL_TEXTURE_2D, id_);

		SetParameters(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

		// load image
		int width, height, nr_channels;
		stbi_set_flip_vertically_on_load(flip_vertically);
		stbi_uc* data = stbi_load(m_path.c_str(), &width, &height, &nr_channels, 0);
		width_ = width;
		height_ = height;
		nr_channels_ = nr_channels;

		GLenum internal_format = 0, data_format = 0;
		if (nr_channels_ == 3) {
			internal_format = GL_RGB8;
			data_format = GL_RGB;
		}
		else if (nr_channels_ == 4)
		{
			internal_format = GL_RGBA8;
			data_format = GL_RGBA;
		}

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, data_format, width_, height_, 0, data_format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			TY_CORE_ERROR("Failed to load texture");
		}
		stbi_image_free(data);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &id_);
	}

	void Texture2D::Bind(uint32_t texture_unit) const
	{
		// activate texture unit before binding
		glActiveTexture(GL_TEXTURE0 + texture_unit);
		glBindTexture(GL_TEXTURE_2D, id_);

		// Alternatively: glBindTextureUnit combines functionality of glActiveTexture and glBindTexture into a single call
		// Example: glBindTextureUnit(texture_unit, id_)
	}

	void Texture2D::Unbind() const
	{
		//set textuer unit back to defaults once configured
		glActiveTexture(GL_TEXTURE0); 
	}

	void Texture2D::SetParameters(uint32_t wrap_s, uint32_t wrap_t, uint32_t min_filter, uint32_t mag_filter)
	{
		glBindTexture(GL_TEXTURE_2D, id_);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	}
}