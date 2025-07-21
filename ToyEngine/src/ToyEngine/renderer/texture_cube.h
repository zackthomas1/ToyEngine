#pragma once
#include "ToyEngine/enum.h"

namespace ToyEngine
{
	class TextureCube
	{
	public:
		virtual ~TextureCube() {}

		virtual uint32_t GetWidth() const { return width_; }
		virtual uint32_t GetHeight() const { return height_; }
		virtual void Bind(uint32_t texture_unit) const = 0;

		// Create cube map from 6 individual face images
		static Ref<TextureCube> Create(const Array<std::string, 6>& face_paths, bool flip_vertically = false);
		
		// Create cube map from single cubemap file (for future use)
		static Ref<TextureCube> Create(const std::string& path, bool flip_vertically = false);

	public:
		uint32_t id_, width_, height_;
		Array<std::string, 6> m_face_paths;
		std::string m_single_path;
		bool m_is_single_file;

	protected:
		TextureCube(const Array<std::string, 6>& face_paths);
		TextureCube(const std::string& path);
		uint32_t nr_channels_;
	};
}