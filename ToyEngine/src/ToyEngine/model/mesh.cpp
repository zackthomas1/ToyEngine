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
        // The vertex and index buffers will be automatically cleaned up when the shared_ptr goes out of scope
	}

	void Mesh::setupMesh()
	{
        // Create vertex and index buffers using the abstraction
        m_vertex_buffer = VertexBuffer::Create(&m_vertices[0], m_vertices.size() * sizeof(Vertex));
        m_index_buffer = IndexBuffer::Create(&m_indices[0], m_indices.size());

        // Gen and bind vertex array
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        // Bind vertex buffer
        m_vertex_buffer->Bind();

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

        // Bind index buffer
        m_index_buffer->Bind();

        // Release vertex array object 
        glBindVertexArray(0);
	}
}