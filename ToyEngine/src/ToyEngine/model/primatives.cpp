#include "pch.h"
#include "primatives.h"

namespace ToyEngine
{
  // CubePrim
  // --------
  Array<float, 288> CubePrim::m_vertices = {
    // positions          // normals            // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0.0f, 1.0f
  };
  BufferLayout CubePrim::m_layout = {
    { eShaderDataType::Vec3, "aPos" },
    { eShaderDataType::Vec3, "aNormal" },
    { eShaderDataType::Vec2, "aTexCoords" },
  };
  Array<uint32_t, 36> CubePrim::m_indices = {
    0,1,2,
    3,4,5,
    
    6,7,8,
    9,10,11,
    
    12,13,14,
    15,16,17,
    
    18,19,20,
    21,22,23,
    
    24,25,26,
    27,28,29,
    
    30,31,32,
    33,34,35,
  };

  // PlanePrim
  // --------
  Array<float, 48> PlanePrim::m_vertices = {
    // positions   // normals                 // texture coords
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,

     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,
  };
  BufferLayout PlanePrim::m_layout = {
    { eShaderDataType::Vec3, "aPos" },
    { eShaderDataType::Vec3, "aNormal" },
    { eShaderDataType::Vec2, "aTexCoords" },
  };
  Array<uint32_t, 6> PlanePrim::m_indices = {
    0,1,2,
    3,4,5
  };

  // TrianglePrim
  // --------
  Array<float, 24> TrianglePrim::m_vertices = {
    // positions          // normals        // texture coords
     0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,  0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f,  0.0f,  // bottom left
     0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 0.5f,  1.0f,  // top 
  };

  BufferLayout TrianglePrim::m_layout = {
    { eShaderDataType::Vec3, "aPos" },
    { eShaderDataType::Vec3, "aNormal" },
    { eShaderDataType::Vec2, "aTexCoords" },
  };
  Array<uint32_t, 3> TrianglePrim::m_indices = {
    0,1,2,
  };

  // SkyBoxPrim
  // --------
  Array<float, 108> SkyBoxPrim::m_vertices = {
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
  BufferLayout SkyBoxPrim::m_layout = {
    { eShaderDataType::Vec3, "aPos" },
  };
  Array<uint32_t, 36> SkyBoxPrim::m_indices = {
    0,1,2,
    3,4,5,

    6,7,8,
    9,10,11,

    12,13,14,
    15,16,17,

    18,19,20,
    21,22,23,

    24,25,26,
    27,28,29,

    30,31,32,
    33,34,35,
  };

  // QuadPrim
  // --------
  Array<float, 24> TextureQuadPrim::m_vertices = {
    // positions	// texCoords
    -1.0f,  1.0f,	0.0f, 1.0f,
    -1.0f, -1.0f,	0.0f, 0.0f,
     1.0f, -1.0f,	1.0f, 0.0f,
     1.0f,  1.0f,	1.0f, 1.0f
  };
  BufferLayout TextureQuadPrim::m_layout = {
    { eShaderDataType::Vec2, "aPos" },
    { eShaderDataType::Vec2, "aTexCoords" },
  };
  Array<uint32_t, 6> TextureQuadPrim::m_indices = {
    0,1,2,
    0,2,3,
  };
}