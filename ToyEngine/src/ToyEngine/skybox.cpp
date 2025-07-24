#include "pch.h"
#include "skybox.h"
#include <glad/glad.h>
#include "renderer/renderer.h"

namespace ToyEngine
{
	float Skybox::s_vertices[108] = {
		// positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	uint32_t Skybox::s_vao = 0;
	uint32_t Skybox::s_vbo = 0;

	Skybox::Skybox(Ref<TextureCube> texture, Ref<Shader> shader) : texture_(texture), m_shader(shader)
	{
		TY_CORE_ASSERT(texture_, "Skybox - Null TextureCube provided");
		TY_CORE_ASSERT(shader, "Skybox - Null Shader provided");

		if (s_vao == 0 || s_vbo == 0) {
			glGenVertexArrays(1, &s_vao);
			glGenBuffers(1, &s_vbo);
			glBindVertexArray(s_vao);
			glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), &s_vertices[0], GL_STATIC_DRAW);
		
			// aPos
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			
			// Set binding back to default
			glBindVertexArray(0);
		}
	}

	Skybox::~Skybox()
	{
		if (s_vao != 0) {
			glDeleteVertexArrays(1, &s_vao);
			s_vao = 0;
		}
		if (s_vbo != 0) {
			glDeleteBuffers(1, &s_vbo);
			s_vbo = 0;
		}
	}

	void Skybox::Render(const glm::mat4& world_transform) const
	{
		// validation
		TY_CORE_ASSERT(m_shader, "Skybox - Null Shader");
		TY_CORE_ASSERT(texture_, "Skybox - Null TextureCube");
		TY_CORE_ASSERT(s_vao != 0, "Skybox - Invalid VAO");

		// Save current OpenGL state
		GLboolean depthMask;
		glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask);
		GLboolean cullFace;
		glGetBooleanv(GL_CULL_FACE, &cullFace);
		GLenum depthFunc;
		glGetIntegerv(GL_DEPTH_FUNC, (GLint*)&depthFunc);

		// Set skybox rendering state
		glDepthMask(GL_FALSE); // Don't write to depth buffer
		glDisable(GL_CULL_FACE);
		glDepthFunc(GL_LEQUAL);

		m_shader->Use();
		m_shader->SetInt("skybox_texture", 0);

		glBindVertexArray(s_vao);

		texture_->Bind(0);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Check for errors
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			TY_CORE_ERROR("Skybox render error: {}", error);
		}

		glBindVertexArray(0);

		// Restore previous OpenGL state
		glDepthMask(depthMask);
		if (cullFace) glEnable(GL_CULL_FACE);
		glDepthFunc(depthFunc);
	}
}