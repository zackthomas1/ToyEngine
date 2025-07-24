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

	void OpenGLRenderAPI::GenVertexArrays(uint32_t size, uint32_t& vao) const
	{
		glGenVertexArrays(1, &vao);
	}
	
	void OpenGLRenderAPI::GenBuffers(uint32_t size, uint32_t& buffer_object) const
	{
		glGenBuffers(1, &buffer_object);
	}

	void OpenGLRenderAPI::BindVertexArray(uint32_t vao) const
	{
		glBindVertexArray(static_cast<GLuint>(vao));
	}

	void OpenGLRenderAPI::BindBuffer(eBufferType buffer_type, uint32_t buffer_object) const
	{
		glBindBuffer(static_cast<GLenum>(buffer_type), buffer_object);
	}

	void OpenGLRenderAPI::BufferData(eBufferType buffer_type, uint32_t size, const void* data) const
	{
		glBufferData(static_cast<GLenum>(buffer_type), size, data, GL_STATIC_DRAW);
	}
	
	void OpenGLRenderAPI::DeleteVertexArray(uint32_t& id) const
	{

		glDeleteVertexArrays(1, &static_cast<GLuint>(id));
	}

	void OpenGLRenderAPI::EnableVertexAttribArray(uint32_t index) const
	{
		glEnableVertexAttribArray(index);
	}

	void OpenGLRenderAPI::VertexAttribPointer(uint32_t location, uint32_t elements, eDataType data_type, uint32_t size, uint32_t offset) const
	{
		glVertexAttribPointer(location, elements, static_cast<GLuint>(data_type), GL_FALSE, size, (void*)offset);
	}

	void OpenGLRenderAPI::DeleteBuffer(uint32_t& id) const
	{
		glDeleteBuffers(1, &id);
	}

	void OpenGLRenderAPI::DrawArrays(ePrimType type, uint32_t start_index, uint32_t vertices) const
	{
		glDrawArrays(static_cast<GLuint>(type), start_index, vertices);

		// Check for errors
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			TY_CORE_ERROR("Skybox render error: {}", error);
		}
	}

	void OpenGLRenderAPI::DrawElements(ePrimType type, uint32_t indices, uint32_t offset) const
	{
		glDrawElements(static_cast<GLuint>(type), indices, GL_UNSIGNED_INT, (void*)offset);

		// Check for errors
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			TY_CORE_ERROR("Skybox render error: {}", error);
		}
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
	void OpenGLRenderAPI::Enable(eParamType cap) const
	{
		glEnable((GLenum)cap);
	}

	void OpenGLRenderAPI::Disable(eParamType cap) const
	{
		glDisable((GLenum)cap);
	}

	void OpenGLRenderAPI::GetBooleanv(eParamType pname, bool* data) const
	{
		glGetBooleanv((GLenum)pname, (GLboolean*)data);
	}
	
	void OpenGLRenderAPI::GetIntegerv(eParamType pname, int* data) const
	{
		glGetIntegerv((GLenum)pname, (GLint*)data);
	}

	void OpenGLRenderAPI::DepthMask(bool flag) const
	{
		glDepthMask(flag);
	}

	void ToyEngine::OpenGLRenderAPI::DepthFunc(eDepthFunc func) const
	{
		glDepthFunc((GLenum)func);
	}
}

