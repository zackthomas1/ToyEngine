#include "pch.h"
#include "plane.h"

#include <glad/glad.h>

namespace ToyEngine
{
	Plane::Plane()
	{
		vertex_count_ = sizeof(vertices_)/ sizeof(float);
	
		// define plane vertex data  
		// generate vertex array object (VAO) to store vertex attributes
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		// create Vertex Buffer Object (VBO)
		glGenBuffers(1, &vbo_);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// uv attribute 
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
}