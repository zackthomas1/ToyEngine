#include "pch.h"
#include "primatives.h"

namespace ToyEngine {
	uint32_t TextureQuad::s_vao = 0;
	uint32_t TextureQuad::s_vbo = 0;
	int TextureQuad::s_ref_count = 0;
	float TextureQuad::s_vertices[24] = {
		// positions	// texCoords
		-1.0f,  1.0f,	0.0f, 1.0f,
		-1.0f, -1.0f,	0.0f, 0.0f,
		 1.0f, -1.0f,	1.0f, 0.0f,

		-1.0f,  1.0f,	0.0f, 1.0f,
		 1.0f, -1.0f,	1.0f, 0.0f,
		 1.0f,  1.0f,	1.0f, 1.0f
	};

	TextureQuad::TextureQuad() {
		s_ref_count++;

		if (s_vao == 0 || s_vbo == 0) 
		{
			RenderCommand::GenVertexArrays(1, s_vao);
			RenderCommand::GenBuffers(1, s_vbo);
			RenderCommand::BindVertexArray(s_vao);
			RenderCommand::BindBuffer(eBufferType::kARRAY_BUFFER, s_vbo);
			RenderCommand::BufferData(eBufferType::kARRAY_BUFFER, sizeof(s_vertices), &s_vertices[0]);

			// aPos
			RenderCommand::EnableVertexAttribArray(0);
			RenderCommand::VertexAttribPointer(0, 2, eDataType::kFLOAT, 4 * sizeof(float), 0);

			// aTexCoords
			RenderCommand::EnableVertexAttribArray(1);
			RenderCommand::VertexAttribPointer(1, 2, eDataType::kFLOAT, 4 * sizeof(float), 2 * sizeof(float));

			// Set binding back to default
			RenderCommand::BindVertexArray(0);
		}
	}
	TextureQuad::~TextureQuad() 
	{
		s_ref_count--;
		if (s_ref_count == 0) {
			if (s_vao != 0) {
				RenderCommand::DeleteVertexArray(s_vao);
				s_vao = 0;
			}
			if (s_vbo != 0) {
				RenderCommand::DeleteBuffer(s_vbo);
				s_vbo = 0;
			}
		}
	}
}