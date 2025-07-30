#pragma once
#include <cstdint>
#include <string>
#include <initializer_list>
#include "ToyEngine/renderer/buffer.h"

namespace ToyEngine
{
	struct CubePrim
	{
		static uint32_t m_vertex_count;
		static float m_vertices[288];
		static BufferLayout m_layout;
	};

	struct PlanePrim {
		static uint32_t m_vertex_count;
		static float m_vertices[48];
		static BufferLayout m_layout;
	};

	struct TrianglePrim {
		static uint32_t m_vertex_count;
		static float m_vertices[24];
		static BufferLayout m_layout;
	};



	class TextureQuadPrim {
	public:
		static float		m_vertices[24];
		static uint32_t		m_indices[6];
		static BufferLayout m_layout;
	};
}