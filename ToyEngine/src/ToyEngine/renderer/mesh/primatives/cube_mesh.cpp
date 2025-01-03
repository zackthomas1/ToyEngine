#include "pch.h"
#include "cube_mesh.h"

#include "ToyEngine/renderer/render_api.h"
#include <glad/glad.h>

namespace ToyEngine
{
	CubeMesh::CubeMesh() {
		vertex_count_ = 36;

		AttribPointerData a_pos = { 3, 5 * sizeof(float), 0 };
		AttribPointerData a_text_coord = { 2, 5 * sizeof(float), 3 * sizeof(float) };
		attribs_.push_back(a_pos);
		attribs_.push_back(a_text_coord);

		RenderAPI::CreateVertexBuffer(&vbo_, vertices_, sizeof(vertices_));
		RenderAPI::CreateVertexAttrib(&vao_, attribs_);
	}
}