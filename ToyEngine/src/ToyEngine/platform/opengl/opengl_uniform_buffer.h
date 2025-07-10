#pragma once
#include "ToyEngine/renderer/uniform_buffer.h"

namespace ToyEngine
{
    class OpenGLUniformBuffer : public UniformBuffer
    {
    public:
        OpenGLUniformBuffer(uint32_t size, uint32_t binding);
        virtual ~OpenGLUniformBuffer();

        virtual void Bind(uint32_t binding) override;
        virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;
        virtual uint32_t GetBufferID() const override { return m_buffer_id; }

    private:
        uint32_t m_buffer_id;
        uint32_t m_size;
    };
}