#include "pch.h"
#include "mesh.h"

#include <glad/glad.h>

namespace ToyEngine
{
	Mesh::~Mesh()
	{
		//de-allocate all resources
		glDeleteVertexArrays(1, &vao_);
		glDeleteBuffers(1, &vbo_);
	}

	void Mesh::Draw() const
	{
		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLES, 0, vertex_count_);
	}
}