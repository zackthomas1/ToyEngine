#include "pch.h"
#include "render_api.h"

#include <glad/glad.h>

namespace ToyEngine
{
	void RenderAPI::ClearSetBackground()
	{
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// Textures

	// Mesh 
	void RenderAPI::BindVertexArray(uint32_t vao)
	{
		glBindVertexArray(static_cast<GLuint>(vao));
	}
	void RenderAPI::DeleteVertexArray(uint32_t id)
	{
		glDeleteVertexArrays(1, &static_cast<GLuint>(id));
	}

	void RenderAPI::CreateVertexBuffer(uint32_t* vbo, float* vertex_array, const int size)
	{
		// create Vertex Buffer Object (VBO)
		glGenBuffers(1, vbo);

		glBindBuffer(GL_ARRAY_BUFFER, *vbo);
		glBufferData(GL_ARRAY_BUFFER, size, vertex_array, GL_STATIC_DRAW);
	}

	void RenderAPI::DeleteBuffer(uint32_t id)
	{
		glDeleteBuffers(1, &static_cast<GLuint>(id));
	}

	void RenderAPI::DrawArrays(uint32_t vertices)
	{
		glDrawArrays(GL_TRIANGLES, 0, vertices); 
	}

	void RenderAPI::DrawIndexed(uint32_t indices)
	{
		glDrawElements(GL_TRIANGLES, (indices), GL_UNSIGNED_INT, 0);
	}

	void RenderAPI::PolygonMode(uint32_t face, uint32_t mode)
	{
		//face options:
		// GL_FRONT_AND_BACK
		
		//mode options:
		//GL_POINT
		//	Polygon vertices that are marked as the start of a boundary edge are drawn as points.
		//	Point attributes such as GL_POINT_SIZE and GL_POINT_SMOOTH control the rasterization of the points.
		//GL_LINE
		//	Boundary edges of the polygon are drawn as line segments.
		//	Line attributes such as GL_LINE_WIDTH and GL_LINE_SMOOTH control the rasterization of the lines.
		//GL_FILL	
		//	The interior of the polygon is filled.
		//	Polygon attributes such as GL_POLYGON_SMOOTH control the rasterization of the polygon.

		glPolygonMode(face, mode);	// render primitives as wireframes
	}
}

