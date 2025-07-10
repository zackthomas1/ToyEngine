#include "pch.h"
#include "mesh.h"

#include "ToyEngine/renderer/render_api.h"
#include <glad/glad.h>

namespace ToyEngine
{
    Mesh::Mesh(Vector<Vertex>& vertices, Vector<uint32_t>& indices, Ref<Material> material) :
		m_vertices(vertices), m_indices(indices), m_material(material)
	{
		setupMesh();
	}

	Mesh::~Mesh()
	{
        RenderCommand::DeleteVertexArray(m_vao);
        RenderCommand::DeleteBuffer(m_vbo);
        RenderCommand::DeleteBuffer(m_ebo);
	}

	void Mesh::setupMesh()
	{
        // Gen buffers
        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ebo);

        // Bind buffers
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        // Set vertice buffer
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

        // Set element array buffer 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(uint32_t), &m_indices[0], GL_STATIC_DRAW);

        // Set vertex attribute pointers
        // --------------
        // aPos
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // aNormals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        // aTexCoords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        // Release vertex array object 
        glBindVertexArray(0);
	}
}