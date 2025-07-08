#pragma once
#include "ToyEngine/enum.h"

namespace ToyEngine
{
	class Texture2D
	{
	public:
		Texture2D(std::string &path, eTextureType type, bool flip_vertically = false);
		~Texture2D();

		void Bind(unsigned int texture_unit) const;
		void Unbind() const;
		void SetParameters(uint32_t wrap_s, uint32_t wrap_t, uint32_t min_filter, uint32_t mag_filter);
	public:
		unsigned int id_, width_, height_;
		std::string m_path;
		eTextureType m_type;
	private:
		unsigned int nr_channels_;
	};
}