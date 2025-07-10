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
        m_data.view = view;
    }

    void MatricesBuffer::SetProjectionMatrix(const glm::mat4& projection)
    {
        m_data.projection = projection;
    }

    void MatricesBuffer::SetModelMatrix(const glm::mat4& model)
    {
        m_data.model = model;
    }

    void MatricesBuffer::UpdateBuffer()
    {
        m_uniform_buffer->SetData(&m_data, sizeof(MatricesUniformBuffer));
    }
}