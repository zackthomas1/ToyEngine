#include "pch.h"
#include "opengl_render_api.h"

#include <glad/glad.h>

namespace ToyEngine
{
    void OpenGLRenderAPI::ClearSetBackground(const glm::vec4& clear_color) const
    {
		glEnable(GL_DEPTH_TEST);
		glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

	void OpenGLRenderAPI::CreateVertexArray(uint32_t& vao) const
	{
		glGenVertexArrays(1, &vao);
	}

	void OpenGLRenderAPI::BindVertexArray(const uint32_t& vao) const
	{
		glBindVertexArray(static_cast<GLuint>(vao));
	}
	
	void OpenGLRenderAPI::DeleteVertexArray(uint32_t& id) const
	{

		glDeleteVertexArrays(1, &static_cast<GLuint>(id));
	}

	void OpenGLRenderAPI::CreateVertexBuffer(uint32_t& vbo, float* vertex_array, const int size) const
	{
		// create Vertex Buffer Object (VBO)
		glGenBuffers(1, &vbo);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size, vertex_array, GL_STATIC_DRAW);
	}

	void OpenGLRenderAPI::DeleteBuffer(uint32_t& id) const
	{
		glDeleteBuffers(1, &id);
	}

	void OpenGLRenderAPI::DrawArrays(uint32_t vertices) const
	{
		glDrawArrays(GL_TRIANGLES, 0, vertices); 
	}

	void OpenGLRenderAPI::DrawIndexed(uint32_t indices) const
	{
		glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);
	}

	void OpenGLRenderAPI::PolygonMode(uint32_t face, uint32_t mode) const
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

