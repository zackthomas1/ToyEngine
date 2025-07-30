#include "pch.h"
#include "primatives.h"

namespace ToyEngine
{
	// CubePrim
	// --------
	uint32_t CubePrim::m_vertex_count = 36;
	float CubePrim::m_vertices[288] = {
		// positions			// normals				// texture coords
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,		1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f
	};
	BufferLayout CubePrim::m_layout = {
		{ eShaderDataType::Vec3, "aPos" },
		{ eShaderDataType::Vec3, "aNormal" },
		{ eShaderDataType::Vec2, "aTexCoords" },
	};

	// CubePrim
	// --------
	uint32_t PlanePrim::m_vertex_count = 6;
	float PlanePrim::m_vertices[48] = {
		// positions			// normals			// texture coords
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,	0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,	1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,	1.0f, 1.0f,

		 0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,	1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,	0.0f, 0.0f,
	};
	BufferLayout PlanePrim::m_layout = {
		{ eShaderDataType::Vec3, "aPos" },
		{ eShaderDataType::Vec3, "aNormal" },
		{ eShaderDataType::Vec2, "aTexCoords" },
	};

	// CubePrim
	// --------
	uint32_t TrianglePrim::m_vertex_count = 3;
	float TrianglePrim::m_vertices[24] = {
		// positions			// normals			// texture coords
		 0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f,  0.0f,	// bottom right
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f,  0.0f,	// bottom left
		 0.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.5f,  1.0f,	// top 
	};

	BufferLayout TrianglePrim::m_layout = {
		{ eShaderDataType::Vec3, "aPos" },
		{ eShaderDataType::Vec3, "aNormal" },
		{ eShaderDataType::Vec2, "aTexCoords" },
	};

	// QuadPrim
	// --------
	float TextureQuadPrim::m_vertices[24] = {
		// positions	// texCoords
		-1.0f,  1.0f,	0.0f, 1.0f,
		-1.0f, -1.0f,	0.0f, 0.0f,
		 1.0f, -1.0f,	1.0f, 0.0f,

		-1.0f,  1.0f,	0.0f, 1.0f,
		 1.0f, -1.0f,	1.0f, 0.0f,
		 1.0f,  1.0f,	1.0f, 1.0f
	};
	uint32_t TextureQuadPrim::m_indices[6] = {
		0,1,2,
		3,4,5
	};
	BufferLayout TextureQuadPrim::m_layout = {
		{ eShaderDataType::Vec2, "aPos" },
		{ eShaderDataType::Vec2, "aTexCoords" },
	};
}
