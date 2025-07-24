#pragma once
#include "ToyEngine/enum.h"

namespace ToyEngine
{
	class RenderAPI
	{
	public:
		// Enforce static class
		// Delete default, copy, and move constructors
		RenderAPI() {}
		virtual ~RenderAPI() {}

		static void Init(eRenderAPI api);
		static Scope<RenderAPI> s_instance;

		// Methods
		virtual void ClearSetBackground(const glm::vec4& clear_color) const = 0;
		virtual void GenVertexArrays(const int size, uint32_t& vao) const = 0;
		virtual void GenBuffers(const int size, uint32_t & buffer_object) const = 0;
		virtual void BindVertexArray(const uint32_t& vao) const = 0;
		virtual void BindBuffer(eBufferType buffer_type, const uint32_t buffer_object) const = 0;
		virtual void BufferData(eBufferType buffer_type, uint32_t size, const void* data) const = 0;
		virtual void DeleteVertexArray(uint32_t& id) const = 0;
		virtual void CreateVertexAttrib(uint32_t location, uint32_t elements, eDataType data_type, uint32_t size, uint32_t offset) const = 0;
		virtual void DeleteBuffer(uint32_t& id) const = 0;
		virtual void DrawArrays(uint32_t vertices) const = 0;
		virtual void DrawIndexed(uint32_t vao) const = 0;
		virtual void PolygonMode(uint32_t face, uint32_t mode) const = 0;
	};

	class RenderCommand
	{
	public:
		// Enforce static class
		// Delete default, copy, and move constructors
		RenderCommand() = delete;
		RenderCommand(const RenderAPI&) = delete;
		RenderCommand(RenderAPI&&) = delete;

		// Delete the assignment operators
		RenderCommand& operator=(const RenderAPI&) = delete;
		RenderCommand& operator=(RenderAPI&&) = delete;

		static void ClearSetBackground(const glm::vec4& clear_color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f)) 
		{ 
			RenderAPI::s_instance->ClearSetBackground(clear_color); 
		}

		static void GenVertexArrays(const int size, uint32_t& vao)
		{
			RenderAPI::s_instance->GenVertexArrays(size, vao);
		}

		static void GenBuffers(const int size, uint32_t& buffer_object)
		{
			RenderAPI::s_instance->GenBuffers(size, buffer_object);
		}

		static void BindVertexArray(const uint32_t& vao)
		{ 
			RenderAPI::s_instance->BindVertexArray(vao); 
		}
		
		static void BindBuffer(eBufferType buffer_type, const uint32_t buffer_object) 
		{
			RenderAPI::s_instance->BindBuffer(buffer_type, buffer_object); 
		}
		
		static void BufferData(eBufferType buffer_type, uint32_t size, const void* data)
		{
			RenderAPI::s_instance->BufferData(buffer_type, size, data);
		}

		static void DeleteVertexArray(uint32_t& id)
		{ 
			RenderAPI::s_instance->DeleteVertexArray(id); 
		}

		static void CreateVertexAttrib(uint32_t location, uint32_t elements, eDataType data_type, uint32_t size, uint32_t offset)
		{
			RenderAPI::s_instance->CreateVertexAttrib(location, elements, data_type, size, offset);
		}

		static void DeleteBuffer(uint32_t& id)
		{ RenderAPI::s_instance->DeleteBuffer(id); }
		static void DrawArrays(uint32_t vertices)
		{ RenderAPI::s_instance->DrawArrays(vertices); }
		static void DrawIndexed(uint32_t vao)
		{ RenderAPI::s_instance->DrawIndexed(vao); }
		static void PolygonMode(uint32_t face, uint32_t mode) 
		{ RenderAPI::s_instance->PolygonMode(face, mode); }
	};
}