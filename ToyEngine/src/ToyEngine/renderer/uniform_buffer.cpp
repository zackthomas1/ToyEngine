#include "pch.h"
#include "uniform_buffer.h"
#include "ToyEngine/renderer/renderer.h"
#include "ToyEngine/platform/opengl/opengl_uniform_buffer.h"

namespace ToyEngine {
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding_point)
	{
		switch (Renderer::API())
		{
		case(eRenderAPI::kOpenGL): {
			return MakeRef<OpenGLUniformBuffer>(size, binding_point);
		}
		default:
			TY_CORE_ERROR("Render API unknown"); 
			return nullptr;
		}
	}
	void UniformManager::CreateBuffer(std::string name, uint32_t size)
	{
		buffer_lib_.emplace(std::make_pair(name, UniformBuffer::Create(size, m_binding_index++)));
	}

	Ref<UniformBuffer> ToyEngine::UniformManager::GetBuffer(const std::string& name) const
	{
		auto it = buffer_lib_.find(name);
		Ref<UniformBuffer> uniform_buffer;
		if (it != buffer_lib_.end()) {
			uniform_buffer = it->second;
		}
		TY_CORE_ASSERT(uniform_buffer, "UniformBuffer '{}' not found in UniformManager", name);
		return uniform_buffer;
	}

	uint32_t ToyEngine::UniformManager::GetBindPoint(const std::string& name) const
	{
		auto it = buffer_lib_.find(name);
		Ref<UniformBuffer> uniform_buffer;
		if (it != buffer_lib_.end()) {
			uniform_buffer = it->second;
		}
		TY_CORE_ASSERT(uniform_buffer, "UniformBuffer '{}' not found in UniformManager", name);
		return uniform_buffer->GetBindPoint();
	}
    bool UniformManager::HasBuffer(const std::string& name) const
    {
        return buffer_lib_.find(name) != buffer_lib_.end();
    }
}