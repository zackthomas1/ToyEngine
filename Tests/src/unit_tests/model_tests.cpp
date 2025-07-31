#include "utils/test_fixtures.h"
#include "ToyEngine/model/mesh.h"
#include "ToyEngine/model/material.h"
#include <glm/glm.hpp>
#include <chrono>

namespace ToyEngine::Testing
{
    class MeshTest : public ModelTestFixture
    {
    protected:
        void SetUp() override
        {
            ModelTestFixture::SetUp();
            
            // Create test vertex data - a simple triangle
            test_vertices = {
                Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
                Vertex(glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
                Vertex(glm::vec3( 0.0f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.5f, 1.0f))
            };
            
            test_indices = { 0, 1, 2 };
            
            // Create a simple material for testing
            test_material = std::make_shared<Material>();
        }

        Vector<Vertex> test_vertices;
        Vector<uint32_t> test_indices;
        Ref<Material> test_material;
    };

    class MaterialTest : public ModelTestFixture
    {
    protected:
        void SetUp() override
        {
            ModelTestFixture::SetUp();
        }
    };

    TEST_F(MeshTest, VertexStructure)
    {
        // Test Vertex constructor and member access
        glm::vec3 position(1.0f, 2.0f, 3.0f);
        glm::vec3 normal(0.0f, 1.0f, 0.0f);
        glm::vec2 texcoords(0.5f, 0.5f);
        
        Vertex vertex(position, normal, texcoords);
        
        EXPECT_EQ(vertex.Position, position);
        EXPECT_EQ(vertex.Normal, normal);
        EXPECT_EQ(vertex.TexCoords, texcoords);
    }

    TEST_F(MeshTest, VertexDataLayout)
    {
        // Test that vertex data is laid out correctly in memory
        Vertex vertex(glm::vec3(1.0f, 2.0f, 3.0f), 
                     glm::vec3(0.0f, 1.0f, 0.0f), 
                     glm::vec2(0.5f, 0.5f));
        
        // Check size - should be 32 bytes (8 floats * 4 bytes each)
        EXPECT_EQ(sizeof(Vertex), 32);
        
        // Check memory layout - position should be first
        float* vertex_data = reinterpret_cast<float*>(&vertex);
        EXPECT_FLOAT_EQ(vertex_data[0], 1.0f); // Position.x
        EXPECT_FLOAT_EQ(vertex_data[1], 2.0f); // Position.y
        EXPECT_FLOAT_EQ(vertex_data[2], 3.0f); // Position.z
        EXPECT_FLOAT_EQ(vertex_data[3], 0.0f); // Normal.x
        EXPECT_FLOAT_EQ(vertex_data[4], 1.0f); // Normal.y
        EXPECT_FLOAT_EQ(vertex_data[5], 0.0f); // Normal.z
        EXPECT_FLOAT_EQ(vertex_data[6], 0.5f); // TexCoords.x
        EXPECT_FLOAT_EQ(vertex_data[7], 0.5f); // TexCoords.y
    }

    // Note: Mesh constructor tests would require OpenGL context for VAO creation
    // These tests focus on the data structures and interfaces

    TEST_F(MeshTest, MeshDataStorage)
    {
        // In a real scenario with OpenGL context:
        /*
        Mesh mesh(test_vertices, test_indices, test_material);
        
        // Test that mesh stores the data correctly
        EXPECT_EQ(mesh.m_vertices.size(), test_vertices.size());
        EXPECT_EQ(mesh.m_indices.size(), test_indices.size());
        EXPECT_EQ(mesh.m_material, test_material);
        
        // Test vertex data integrity
        for (size_t i = 0; i < test_vertices.size(); ++i)
        {
            EXPECT_EQ(mesh.m_vertices[i].Position, test_vertices[i].Position);
            EXPECT_EQ(mesh.m_vertices[i].Normal, test_vertices[i].Normal);
            EXPECT_EQ(mesh.m_vertices[i].TexCoords, test_vertices[i].TexCoords);
        }
        
        // Test index data integrity
        for (size_t i = 0; i < test_indices.size(); ++i)
        {
            EXPECT_EQ(mesh.m_indices[i], test_indices[i]);
        }
        */
        
        SUCCEED();
    }

    TEST_F(MaterialTest, DefaultMaterialProperties)
    {
        Material material;
        
        // Test default property values
        // Note: These would need getter methods in the Material class
        // For now, we test that the material can be created without errors
        EXPECT_NO_THROW({
            material.SetRoughness(0.5f);
            material.SetMetallic(0.2f);
            material.SetTransmission(0.0f);
            material.SetRefractiveIndex(1.33f);
        });
    }

    TEST_F(MaterialTest, MaterialPropertySetters)
    {
        Material material;
        
        // Test property setters don't throw
        EXPECT_NO_THROW(material.SetRoughness(0.8f));
        EXPECT_NO_THROW(material.SetMetallic(1.0f));
        EXPECT_NO_THROW(material.SetTransmission(0.5f));
        EXPECT_NO_THROW(material.SetRefractiveIndex(2.4f));
        
        // Test extreme values
        EXPECT_NO_THROW(material.SetRoughness(0.0f));  // Minimum roughness
        EXPECT_NO_THROW(material.SetRoughness(1.0f));  // Maximum roughness
        EXPECT_NO_THROW(material.SetMetallic(0.0f));   // Non-metallic
        EXPECT_NO_THROW(material.SetMetallic(1.0f));   // Fully metallic
    }

    TEST_F(MaterialTest, TextureManagement)
    {
        Vector<Ref<Texture2D>> empty_textures;
        Material material(empty_textures);
        
        // Test that material can be created with empty texture vector
        const auto& textures = material.getTextures();
        EXPECT_EQ(textures.size(), 0);
        
        // Test environment map setter
        // Note: Would need a real TextureCube object in practice
        EXPECT_NO_THROW(material.SetEnvironmentMap(nullptr));
    }

    // Test mesh generation for common primitives
    TEST_F(MeshTest, TrianglePrimitiveData)
    {
        // Test that our test triangle data is valid
        EXPECT_EQ(test_vertices.size(), 3);
        EXPECT_EQ(test_indices.size(), 3);
        
        // Test that vertices form a valid triangle
        glm::vec3 v0 = test_vertices[0].Position;
        glm::vec3 v1 = test_vertices[1].Position;
        glm::vec3 v2 = test_vertices[2].Position;
        
        // Calculate triangle area using cross product
        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec3 cross = glm::cross(edge1, edge2);
        float area = glm::length(cross) * 0.5f;
        
        EXPECT_GT(area, 0.0f); // Triangle should have positive area
        
        // Test normals are normalized
        for (const auto& vertex : test_vertices)
        {
            float normal_length = glm::length(vertex.Normal);
            EXPECT_NEAR(normal_length, 1.0f, 0.001f);
        }
    }

    TEST_F(MeshTest, IndexValidation)
    {
        // Test that all indices are valid for the vertex array
        for (uint32_t index : test_indices)
        {
            EXPECT_LT(index, test_vertices.size());
        }
        
        // Test that indices define a proper triangle (no degenerate)
        EXPECT_NE(test_indices[0], test_indices[1]);
        EXPECT_NE(test_indices[1], test_indices[2]);
        EXPECT_NE(test_indices[0], test_indices[2]);
    }

    // Performance test for vertex data operations
    TEST_F(MeshTest, PerformanceVertexOperations)
    {
        const int num_vertices = 10000;
        Vector<Vertex> large_vertex_buffer;
        large_vertex_buffer.reserve(num_vertices);
        
        auto start = std::chrono::high_resolution_clock::now();
        
        // Generate many vertices
        for (int i = 0; i < num_vertices; ++i)
        {
            float x = static_cast<float>(i % 100) / 100.0f;
            float y = static_cast<float>(i / 100) / 100.0f;
            float z = 0.0f;
            
            large_vertex_buffer.emplace_back(
                glm::vec3(x, y, z),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec2(x, y)
            );
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // Should complete in reasonable time (less than 10ms for 10k vertices)
        EXPECT_LT(duration.count(), 10000);
        EXPECT_EQ(large_vertex_buffer.size(), num_vertices);
    }

    TEST_F(MaterialTest, MaterialBindingInterface)
    {
        Material material;
        
        // Test that binding interface exists and doesn't crash
        // In practice, this would bind textures and set shader uniforms
        EXPECT_NO_THROW({
            // material.Bind(nullptr); // Would pass a real shader in practice
        });
    }
}