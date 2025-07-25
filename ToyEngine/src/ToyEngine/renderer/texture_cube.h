#pragma once
#include "ToyEngine/core.h"

namespace ToyEngine 
{
	const int CUBE_SIDES = 6;

	class TextureCube 
	{
	public:
		virtual ~TextureCube() {} 
		
		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void GenerateMipMaps() = 0;

		uint32_t id() const { return id_; }
		int GeMaxMipLevel() const { return max_mip_level_; }

		static Ref<TextureCube> Create(Array<std::string, CUBE_SIDES>& files);
	protected:
		TextureCube() : id_(0) {};
		uint32_t id_ = 0;
		int max_mip_level_ = 0;
	};
}