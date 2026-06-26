#include "pch.h"
#include "mesh.h"
#include "ToyEngine/enum.h"
#include "ToyEngine/renderer/render_api.h"

namespace ToyEngine
{
  Mesh::Mesh(Vector<Vertex>& vertices, Vector<uint32_t>& indices, Ref<Material> material) :
    m_vertices(vertices), m_indices(indices), m_material(material)
  {
    Setup();
  }

  Mesh::~Mesh()
  {
    if (m_vao) {
      m_vao.reset(); // This will trigger proper cleanup
    }
  }

  void Mesh::Setup()
  {
    // Check for empty data
    if (m_vertices.empty() || m_indices.empty()) {
      TY_CORE_ERROR("Cannot create mesh with empty vertices or indices");
      return;
    }

    m_vao = VertexArray::Create();
    Ref<VertexBuffer> buffer = VertexBuffer::Create(m_vertices.data(), m_vertices.size() * sizeof(Vertex));
    BufferLayout layout = BufferLayout {
      { eShaderDataType::Vec3, "aPos", },
      { eShaderDataType::Vec3, "aNormal" },
      { eShaderDataType::Vec2, "aTexCoords" }, };
    buffer->SetLayout(layout);
    Ref<IndexBuffer> indices = IndexBuffer::Create(m_indices.data(), m_indices.size());
    m_vao->AddBuffer(buffer);
    m_vao->SetIndexBuffer(indices);
  }
}