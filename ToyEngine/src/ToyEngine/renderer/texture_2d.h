#pragma once
#include "ToyEngine/enum.h"

namespace ToyEngine
{
	class Texture2D
	{
	public:
		Texture2D(std::string &path, eTextureType type, bool flip_vertically = false);
		~Texture2D();

		virtual uint32_t GetWidth() const { return width_; }
		virtual uint32_t GetHight() const { return height_;}
		virtual void Bind(uint32_t texture_unit) const = 0;
	public:
		uint32_t id_, width_, height_;
		std::string m_path;
		eTextureType m_type;
	protected:
		uint32_t nr_channels_;
	};
}