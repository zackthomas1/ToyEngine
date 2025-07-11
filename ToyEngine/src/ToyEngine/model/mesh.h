#pragma once
#include "ToyEngine/model/material.h"
#include "ToyEngine/renderer/vertex_buffer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace ToyEngine
{
	struct Vertex {
		//Size: 32 bytes (8 floats * 4 bytes each)
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;

		Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 texcoords) :
			Position(pos), Normal(norm), TexCoords(texcoords) {}
	};

	class Mesh
	{
	public:
		Mesh(Vector<Vertex>& vertices, Vector<uint32_t>& indices, Ref<Material> material);
		virtual ~Mesh();

	public:
		// mesh data 
		uint32_t m_vao;
		Ref<VertexBuffer> m_vertex_buffer;
		Ref<IndexBuffer> m_index_buffer;
		Vector<Vertex>		m_vertices;
		Vector<uint32_t>	m_indices;
		Ref<Material>		m_material;
	private:
		void setupMesh();
	};
}
