#pragma once
#include <glm/glm.hpp>

namespace ToyEngine
{
    // Abstract base class for uniform buffer objects
    class UniformBuffer
    {
    public:
        virtual ~UniformBuffer() = default;

        // Bind the uniform buffer to the specified binding point
        virtual void Bind(uint32_t binding) = 0;

        // Update the entire buffer with new data
        virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

        // Get the underlying buffer ID
        virtual uint32_t GetBufferID() const = 0;

        // Factory method to create platform-specific uniform buffer
        static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding = 0);

    protected:
        UniformBuffer() = default;
    };

    // Specific uniform buffer for matrices (view, projection, model)
    struct MatricesUniformBuffer
    {
        glm::mat4 view;
        glm::mat4 projection;
        glm::mat4 model;

        MatricesUniformBuffer() 
            : view(1.0f), projection(1.0f), model(1.0f) {}
    };

    // Utility class to manage matrices uniform buffer
    class MatricesBuffer
    {
    public:
        MatricesBuffer();
        ~MatricesBuffer() = default;

        void Bind();
        void SetViewMatrix(const glm::mat4& view);
        void SetProjectionMatrix(const glm::mat4& projection);
        void SetModelMatrix(const glm::mat4& model);
        void UpdateBuffer();
        void UpdateViewProjection(); // Only update view and projection matrices
        void UpdateModel(); // Only update model matrix

        static const uint32_t BINDING_POINT = 0;

    private:
        MatricesUniformBuffer m_data;
        Ref<UniformBuffer> m_uniform_buffer;
        bool m_view_projection_dirty = true;
        bool m_model_dirty = true;
    };
}