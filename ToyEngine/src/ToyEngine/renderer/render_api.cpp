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

	void RenderAPI::ShaderUse(const Shader* shader)
	{
		glUseProgram(shader->id());
	}

	// Textures

	// Mesh 
	void RenderAPI::CreateVertexBuffer(unsigned int* vbo, float* vertex_array, const int size)
	{
		// create Vertex Buffer Object (VBO)
		glGenBuffers(1, vbo);

		glBindBuffer(GL_ARRAY_BUFFER, *vbo);
		glBufferData(GL_ARRAY_BUFFER, size, vertex_array, GL_STATIC_DRAW);
	}

	void RenderAPI::CreateVertexAttrib(unsigned int* vao, const std::vector<AttribPointerData>& attributes)
	{
		// generate vertex array object (VAO) to store vertex attributes
		glGenVertexArrays(1, vao);
		glBindVertexArray(*vao);

		// define attribute pointers
		for (int i = 0; i < attributes.size(); i++)
		{
			glVertexAttribPointer(i, attributes[i].size, GL_FLOAT, GL_FALSE, attributes[i].stride, (void*)attributes[i].offset); 
			glEnableVertexAttribArray(i);
		}
	}

	void RenderAPI::DeleteMesh(Mesh* mesh)
	{
		GLuint glVAO = static_cast<GLuint>(mesh->GetVAO()); 
		GLuint glVBO = static_cast<GLuint>(mesh->GetVBO());

		glDeleteVertexArrays(1, &glVAO);
		glDeleteBuffers(1, &glVBO);
	}

	void RenderAPI::DrawArrays(std::shared_ptr<Mesh> mesh)
	{
		glBindVertexArray(mesh->GetVAO());
		glDrawArrays(GL_TRIANGLES, 0, mesh->GetVertexCount());
	}
}

