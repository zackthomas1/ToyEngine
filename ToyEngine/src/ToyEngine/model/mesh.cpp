#include "pch.h"
#include "mesh.h"
#include "ToyEngine/enum.h"
#include "ToyEngine/renderer/render_api.h"

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

		m_vao = 0;
		m_vbo = 0;
		m_ebo = 0;
	}

	void Mesh::setupMesh()
	{
		// Generate buffers
		RenderCommand::GenVertexArrays(1, m_vao);
		RenderCommand::GenBuffers(1, m_vbo);
		RenderCommand::GenBuffers(1, m_ebo);
		RenderCommand::BindVertexArray(m_vao);

		// Fill buffers
		RenderCommand::BindBuffer(eBufferType::kARRAY_BUFFER, m_vbo); 
		RenderCommand::BufferData(eBufferType::kARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0]);
		RenderCommand::BindBuffer(eBufferType::kELEMENT_ARRAY_BUFFER, m_ebo);
		RenderCommand::BufferData(eBufferType::kELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(uint32_t), &m_indices[0]);

		// Set vertex attribute pointers
		// aPos
		RenderCommand::EnableVertexAttribArray(0);
		RenderCommand::VertexAttribPointer(0, 3, eDataType::kFLOAT, sizeof(Vertex), 0);
		// aNormals
		RenderCommand::EnableVertexAttribArray(1);
		RenderCommand::VertexAttribPointer(1, 3, eDataType::kFLOAT, sizeof(Vertex), offsetof(Vertex, Normal));
		// aTexCoords
		RenderCommand::EnableVertexAttribArray(2);
		RenderCommand::VertexAttribPointer(2, 2, eDataType::kFLOAT, sizeof(Vertex), offsetof(Vertex, TexCoords));

		// Release vertex array object 
		RenderCommand::BindVertexArray(0);
	}
}