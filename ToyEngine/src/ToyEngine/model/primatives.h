#pragma once
#include <cstdint>
#include <string>
#include <initializer_list>
#include "ToyEngine/renderer/buffer.h"

namespace ToyEngine
{
	class CubePrim {
	public:
		static Array<float, 288>	m_vertices;
		static BufferLayout			m_layout;
		static Array<uint32_t, 36>	m_indices;
	};


	class PlanePrim {
	public:
		static Array<float, 48>		m_vertices;
		static BufferLayout			m_layout;
		static Array<uint32_t, 6>	m_indices;
	};

	class TrianglePrim {
	public:
		static Array<float, 24>		m_vertices;
		static BufferLayout			m_layout;
		static Array < uint32_t, 3> m_indices;
	};

	class SkyBoxPrim {
	public:
		static Array<float, 108>	m_vertices;
		static BufferLayout			m_layout;
		static Array<uint32_t, 36>	m_indices;
	};

	class TextureQuadPrim {
	public:
		static Array<float, 24>		m_vertices;
		static BufferLayout			m_layout;
		static Array<uint32_t, 6>	m_indices;
	};
}