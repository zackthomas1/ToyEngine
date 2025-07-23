#pragma once
#include "ToyEngine/enum.h"

namespace ToyEngine
{
	class Texture2D
	{
	public:
		virtual ~Texture2D() {}

		virtual uint32_t GetWidth() const { return width_; }
		virtual uint32_t GetHeight() const { return height_;}
		virtual void Bind(uint32_t texture_unit) const = 0;

		static Ref<Texture2D> Create(const std::string& path, eTextureType type, bool flip_vertically = false);
	public:
		uint32_t id_, width_, height_;
		std::string m_path;
		eTextureType m_type;
	protected:
		Texture2D(const std::string& path, eTextureType type) : m_path(path), m_type(type) { }
		uint32_t nr_channels_;
	};
}