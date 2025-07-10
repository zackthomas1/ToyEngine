#include "pch.h"
#include "uniform_buffer.h"

#include "ToyEngine/renderer/render_api.h"

#ifdef TY_PLATFORM_OPENGL
    #include "ToyEngine/platform/opengl/opengl_uniform_buffer.h"
#endif

namespace ToyEngine
{
    Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
    {
#ifdef TY_PLATFORM_OPENGL
        return MakeRef<OpenGLUniformBuffer>(size, binding);
#else
        TY_CORE_ASSERT(false, "Unknown RenderAPI!");
        return nullptr;
#endif
    }

    MatricesBuffer::MatricesBuffer()
    {
        m_uniform_buffer = UniformBuffer::Create(sizeof(MatricesUniformBuffer), BINDING_POINT);
        UpdateBuffer();
    }

    void MatricesBuffer::Bind()
    {
        m_uniform_buffer->Bind(BINDING_POINT);
    }

    void MatricesBuffer::SetViewMatrix(const glm::mat4& view)
    {
        if (m_data.view != view) {
            m_data.view = view;
            m_view_projection_dirty = true;
        }
    }

    void MatricesBuffer::SetProjectionMatrix(const glm::mat4& projection)
    {
        if (m_data.projection != projection) {
            m_data.projection = projection;
            m_view_projection_dirty = true;
        }
    }

    void MatricesBuffer::SetModelMatrix(const glm::mat4& model)
    {
        if (m_data.model != model) {
            m_data.model = model;
            m_model_dirty = true;
        }
    }

    void MatricesBuffer::UpdateBuffer()
    {
        m_uniform_buffer->SetData(&m_data, sizeof(MatricesUniformBuffer));
        m_view_projection_dirty = false;
        m_model_dirty = false;
    }

    void MatricesBuffer::UpdateViewProjection()
    {
        if (m_view_projection_dirty) {
            // Update only view and projection matrices (first 32 bytes)
            m_uniform_buffer->SetData(&m_data.view, sizeof(glm::mat4) * 2, 0);
            m_view_projection_dirty = false;
        }
    }

    void MatricesBuffer::UpdateModel()
    {
        if (m_model_dirty) {
            // Update only model matrix (bytes 32-47)
            m_uniform_buffer->SetData(&m_data.model, sizeof(glm::mat4), sizeof(glm::mat4) * 2);
            m_model_dirty = false;
        }
    }
}