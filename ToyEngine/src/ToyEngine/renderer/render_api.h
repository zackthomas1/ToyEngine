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
		virtual void GenVertexArrays(uint32_t size, uint32_t& vao) const = 0;
		virtual void GenBuffers(uint32_t size, uint32_t & buffer_object) const = 0;
		virtual void BindVertexArray(uint32_t vao) const = 0;
		virtual void BindBuffer(eBufferType buffer_type, uint32_t buffer_object) const = 0;
		virtual void BufferData(eBufferType buffer_type, uint32_t size, const void* data) const = 0;
		virtual void DeleteVertexArray(uint32_t& id) const = 0;
		virtual void EnableVertexAttribArray(uint32_t index) const = 0;
		virtual void VertexAttribPointer(uint32_t location, uint32_t elements, eDataType data_type, uint32_t size, uint32_t offset) const = 0;
		virtual void DeleteBuffer(uint32_t& id) const = 0;
		virtual void DrawArrays(ePrimType type, uint32_t start_index, uint32_t vertices) const = 0;
		virtual void DrawElements(ePrimType type, uint32_t indices, uint32_t offset) const = 0;
		virtual void PolygonMode(uint32_t face, uint32_t mode) const = 0;
		virtual void Enable(eParamType cap) const = 0;
		virtual void Disable(eParamType cap) const = 0;
		virtual void GetBooleanv(eParamType pname, bool* data) const = 0;
		virtual void GetIntegerv(eParamType pname, int* data) const = 0;
		virtual void DepthMask(bool flag) const = 0; 
		virtual void DepthFunc(eDepthFunc func) const = 0;
		virtual void CullFace(eCullFaceMode mode) const = 0;
		virtual void FrontFace(eFrontFace mode) const = 0;
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

		static void GenVertexArrays(uint32_t size, uint32_t& vao)
		{
			RenderAPI::s_instance->GenVertexArrays(size, vao);
		}

		static void GenBuffers(uint32_t size, uint32_t& buffer_object)
		{
			RenderAPI::s_instance->GenBuffers(size, buffer_object);
		}

		static void BindVertexArray(uint32_t vao)
		{ 
			RenderAPI::s_instance->BindVertexArray(vao); 
		}
		
		static void BindBuffer(eBufferType buffer_type, uint32_t buffer_object) 
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

		static void EnableVertexAttribArray(uint32_t index)
		{
			RenderAPI::s_instance->EnableVertexAttribArray(index);
		}

		static void VertexAttribPointer(uint32_t location, uint32_t elements, eDataType data_type, uint32_t size, uint32_t offset)
		{
			RenderAPI::s_instance->VertexAttribPointer(location, elements, data_type, size, offset);
		}

		static void DeleteBuffer(uint32_t& id)
		{ 
			RenderAPI::s_instance->DeleteBuffer(id); 
		}
		
		static void DrawArrays(ePrimType type, uint32_t start_index, uint32_t vertices)
		{ 
			RenderAPI::s_instance->DrawArrays(type, start_index, vertices);
		}
		
		static void DrawElements(ePrimType type, uint32_t indices, uint32_t offset)
		{ 
			RenderAPI::s_instance->DrawElements(type, indices, offset); 
		}
		
		static void PolygonMode(uint32_t face, uint32_t mode) 
		{ RenderAPI::s_instance->PolygonMode(face, mode); }

		static void Enable(eParamType cap)
		{
			RenderAPI::s_instance->Enable(cap);
		}

		static void Disable(eParamType cap)
		{
			RenderAPI::s_instance->Disable(cap);
		}

		static void GetBooleanv(eParamType pname, bool* data)
		{
			RenderAPI::s_instance->GetBooleanv(pname, data);
		}

		static void GetIntegerv(eParamType pname, int* data)
		{
			RenderAPI::s_instance->GetIntegerv(pname, data);
		}

		static void DepthMask(bool flag)
		{
			RenderAPI::s_instance->DepthMask(flag);
		}

		static void DepthFunc(eDepthFunc func)
		{
			RenderAPI::s_instance->DepthFunc(func);
		}

		static void CullFace(eCullFaceMode mode)
		{
			RenderAPI::s_instance->CullFace(mode);
		}

		static void FrontFace(eFrontFace mode)
		{
			RenderAPI::s_instance->FrontFace(mode);
		}
	};
}