#pragma once
#include <cstdint>
#include "ToyEngine/renderer/render_api.h"

namespace ToyEngine
{
	struct CubePrim
	{
		const uint32_t vertex_count = 36;
		const float vertices[288] = {
			// positions			// normals			// texture coords
			-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,	0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,	1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,	0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,	0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,	0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,	1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,	1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,	1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,	0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,	0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, -	1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -	1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -	1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -	1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -	1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -	1.0f,  0.0f,  0.0f,	1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,	1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,	1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,	1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,	0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,	1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,	1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,	1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,	0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,	0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,	1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,	0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,	0.0f, 1.0f
		};
	};

	struct PlanePrim {
		const uint32_t vertex_count = 6;
		const float vertices[48] = {
			// positions			// normals			// texture coords
			-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,	0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,	1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,	1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,	1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,	0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,	0.0f, 0.0f,
		};
	};

	struct TrianglePrim {
		const uint32_t vertex_count = 3;
		float vertices_[24] = {
			// positions			// normals			// texture coords
			 0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f,  0.0f,	// bottom right
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f,  0.0f,	// bottom left
			 0.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.5f,  1.0f,	// top 		
		};
	};

	class TextureQuad {
	public:
		TextureQuad();
		~TextureQuad();

		static uint32_t s_vao, s_vbo;
		static const uint32_t s_vertex_count = 6;
	private:
		static int s_ref_count;
		static float s_vertices[24];
	};
}