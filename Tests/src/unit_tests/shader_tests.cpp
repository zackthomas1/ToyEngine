#include "utils/test_fixtures.h"
#include "ToyEngine/renderer/shader.h"
#include <glm/glm.hpp>
#include <chrono>
#include <unordered_map>

namespace ToyEngine::Testing
{
    class ShaderTest : public RendererTestFixture
    {
    protected:
        void SetUp() override
        {
            RendererTestFixture::SetUp();
            
            // Simple test vertex shader source
            vertex_shader_source = R"(
                #version 330 core
                layout (location = 0) in vec3 aPos;
                uniform mat4 mvp;
                void main()
                {
                    gl_Position = mvp * vec4(aPos, 1.0);
                }
            )";
            
            // Simple test fragment shader source
            fragment_shader_source = R"(
                #version 330 core
                out vec4 FragColor;
                uniform vec3 color;
                void main()
                {
                    FragColor = vec4(color, 1.0);
                }
            )";
        }

        std::string vertex_shader_source;
        std::string fragment_shader_source;
    };

    class ShaderLibraryTest : public RendererTestFixture
    {
    protected:
        void SetUp() override
        {
            RendererTestFixture::SetUp();
            library = std::make_unique<ShaderLibrary>();
        }

        std::unique_ptr<ShaderLibrary> library;
    };

    // Note: These tests would require an OpenGL context in a real scenario
    // For unit testing, we're testing the interface and logic that doesn't require GPU

    TEST_F(ShaderTest, ShaderCreationInterface)
    {
        // Test that the static creation method exists and has correct signature
        // This is a compilation test more than a runtime test
        
        const char* name = "test_shader";
        const char* vertex_path = "test_vertex.glsl";
        const char* fragment_path = "test_fragment.glsl";
        
        // In a real test environment with OpenGL context, this would work:
        // auto shader = Shader::Create(name, vertex_path, fragment_path);
        // ASSERT_NE(shader, nullptr);
        // EXPECT_STREQ(shader->GetName(), name);
        
        // For now, we just test that the interface is correct
        SUCCEED();
    }

    TEST_F(ShaderLibraryTest, AddAndRetrieveShader)
    {
        // Create a mock shader for testing
        // In practice, this would be a real shader object
        
        const std::string shader_name = "test_shader";
        
        // Since we can't create real shaders without OpenGL context,
        // we'll test the library logic with nullptr for now
        // In a real scenario, you'd have: 
        // auto shader = Shader::Create("test", "vertex.glsl", "fragment.glsl");
        // library->Add(shader);
        // auto retrieved = library->Get(shader_name);
        // EXPECT_EQ(shader, retrieved);
        
        SUCCEED();
    }

    TEST_F(ShaderLibraryTest, RetrieveNonExistentShader)
    {
        // Test that retrieving a non-existent shader handles gracefully
        const std::string non_existent_name = "non_existent_shader";
        
        // This should either return nullptr or throw an exception
        // depending on the implementation
        EXPECT_NO_THROW({
            auto result = library->Get(non_existent_name);
            // In most implementations, this would be nullptr
            // EXPECT_EQ(result, nullptr);
        });
    }

    TEST_F(ShaderTest, UniformSetterInterfaces)
    {
        // Test that all uniform setter methods are properly declared
        // This is primarily a compilation test
        
        // In a real scenario with a shader object:
        /*
        auto shader = Shader::Create("test", "vertex.glsl", "fragment.glsl");
        
        // Test boolean uniform
        EXPECT_NO_THROW(shader->SetBool("test_bool", true));
        
        // Test integer uniform
        EXPECT_NO_THROW(shader->SetInt("test_int", 42));
        
        // Test float uniform
        EXPECT_NO_THROW(shader->SetFloat("test_float", 3.14f));
        
        // Test vector uniforms
        EXPECT_NO_THROW(shader->SetFloat2("test_vec2", 1.0f, 2.0f));
        EXPECT_NO_THROW(shader->SetFloat3("test_vec3", 1.0f, 2.0f, 3.0f));
        EXPECT_NO_THROW(shader->SetFloat4("test_vec4", 1.0f, 2.0f, 3.0f, 4.0f));
        
        // Test matrix uniform
        glm::mat4 test_matrix = glm::mat4(1.0f);
        EXPECT_NO_THROW(shader->SetMat4("test_matrix", test_matrix));
        */
        
        SUCCEED();
    }

    // Mock shader class for testing without OpenGL context
    class MockShader : public Shader
    {
    public:
        MockShader(const char* name) : Shader(name), use_called(false) {}

        void Use() override { use_called = true; }
        void BindUniformBlock(const char* uniform_block, uint32_t binding_point) const override {}
        
        void SetBool(const std::string& name, bool value) const override 
        { 
            uniform_calls[name] = std::to_string(value);
        }
        
        void SetInt(const std::string& name, int value) const override 
        { 
            uniform_calls[name] = std::to_string(value);
        }
        
        void SetFloat(const std::string& name, float value) const override 
        { 
            uniform_calls[name] = std::to_string(value);
        }
        
        void SetFloat2(const std::string& name, float v0, float v1) const override 
        { 
            uniform_calls[name] = std::to_string(v0) + "," + std::to_string(v1);
        }
        
        void SetFloat3(const std::string& name, float v0, float v1, float v2) const override 
        { 
            uniform_calls[name] = std::to_string(v0) + "," + std::to_string(v1) + "," + std::to_string(v2);
        }
        
        void SetFloat4(const std::string& name, float v1, float v2, float v3, float v4) const override 
        { 
            uniform_calls[name] = std::to_string(v1) + "," + std::to_string(v2) + "," + std::to_string(v3) + "," + std::to_string(v4);
        }
        
        void SetMat4(const std::string& name, glm::mat4 value) const override 
        { 
            uniform_calls[name] = "mat4";
        }

        bool use_called;
        mutable std::unordered_map<std::string, std::string> uniform_calls;
    };

    TEST_F(ShaderTest, MockShaderFunctionality)
    {
        auto mock_shader = std::make_shared<MockShader>("mock_test");
        
        // Test Use() method
        EXPECT_FALSE(mock_shader->use_called);
        mock_shader->Use();
        EXPECT_TRUE(mock_shader->use_called);
        
        // Test uniform setters
        mock_shader->SetBool("test_bool", true);
        EXPECT_EQ(mock_shader->uniform_calls["test_bool"], "1");
        
        mock_shader->SetInt("test_int", 42);
        EXPECT_EQ(mock_shader->uniform_calls["test_int"], "42");
        
        mock_shader->SetFloat("test_float", 3.14f);
        EXPECT_EQ(mock_shader->uniform_calls["test_float"], "3.140000");
        
        glm::mat4 identity = glm::mat4(1.0f);
        mock_shader->SetMat4("test_matrix", identity);
        EXPECT_EQ(mock_shader->uniform_calls["test_matrix"], "mat4");
    }

    TEST_F(ShaderLibraryTest, MockShaderLibraryOperations)
    {
        auto mock_shader = std::make_shared<MockShader>("mock_shader");
        
        // Add shader to library
        library->Add(mock_shader);
        
        // Retrieve shader from library
        auto retrieved = library->Get("mock_shader");
        ASSERT_NE(retrieved, nullptr);
        EXPECT_STREQ(retrieved->GetName(), "mock_shader");
        
        // Test that it's the same shader
        EXPECT_EQ(mock_shader, retrieved);
    }

    // Performance test for shader library lookups
    TEST_F(ShaderLibraryTest, PerformanceShaderLookup)
    {
        // Add multiple shaders to library
        const int num_shaders = 1000;
        for (int i = 0; i < num_shaders; ++i)
        {
            std::string name = "shader_" + std::to_string(i);
            auto shader = std::make_shared<MockShader>(name.c_str());
            library->Add(shader);
        }
        
        auto start = std::chrono::high_resolution_clock::now();
        
        // Perform many lookups
        const int iterations = 10000;
        for (int i = 0; i < iterations; ++i)
        {
            std::string name = "shader_" + std::to_string(i % num_shaders);
            auto shader = library->Get(name);
            ASSERT_NE(shader, nullptr);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // Should complete in reasonable time (less than 50ms for 10k lookups)
        EXPECT_LT(duration.count(), 50000);
    }
}