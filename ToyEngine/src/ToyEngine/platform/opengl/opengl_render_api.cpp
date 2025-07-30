#include "pch.h"
#include "opengl_render_api.h"

#include <glad/glad.h>

namespace ToyEngine
{
    void OpenGLRenderAPI::ClearSetBackground(const glm::vec4& clear_color) const
    {
		glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

	void OpenGLRenderAPI::BindTexture(eSamplerType texture_type, uint32_t texture_id) const
	{
		glBindTexture(static_cast<GLenum>(texture_type), texture_id);
	}

	void ToyEngine::OpenGLRenderAPI::DrawElements(ePrimType type, const VertexArray* vao) const
	{
		glDrawElements(static_cast<GLuint>(type), vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, (void*)0);

		// Check for errors
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			TY_CORE_ERROR("draw elements error: {}", error);
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
	void OpenGLRenderAPI::CullFace(eParamType face_mode) const
	{
		TY_CORE_ASSERT(face_mode == eParamType::kFRONT || face_mode == eParamType::kBACK, "Face culling mode set to an invalid state.");
		glCullFace((GLenum)face_mode);

	}
	void OpenGLRenderAPI::FrontFace(eParamType winding_order) const
	{
		TY_CORE_ASSERT(winding_order == eParamType::kCW || winding_order == eParamType::kCCW, "Front face mode set to an invalid state.");
		glFrontFace((GLenum)winding_order);
	}
}

