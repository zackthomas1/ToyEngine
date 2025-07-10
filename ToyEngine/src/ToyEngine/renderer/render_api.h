#pragma once
#include "ToyEngine/enum.h"
#include <glm/glm.hpp>
#include <glm/vec4.hpp>

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
		virtual void BindTexture2D() const = 0;
		virtual void CreateVertexArray(uint32_t& vao) const = 0;
		virtual void BindVertexArray(const uint32_t& vao) const = 0;
		virtual void DeleteVertexArray(uint32_t& id) const = 0;
		virtual void CreateVertexBuffer(uint32_t& vbo, float* vertex_array, const int size) const = 0;
		virtual void CreateVertexAttrib(uint32_t& vao) const = 0;
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
			{ RenderAPI::s_instance->ClearSetBackground(clear_color); }
		static void BindTexture2D()
			{ RenderAPI::s_instance->BindTexture2D(); }
		static void CreateVertexArray(uint32_t& vao)
			{ RenderAPI::s_instance->CreateVertexArray(vao); }
		static void BindVertexArray(const uint32_t& vao)
			{ RenderAPI::s_instance->BindVertexArray(vao); }
		static void DeleteVertexArray(uint32_t& id)
			{ RenderAPI::s_instance->DeleteVertexArray(id); }
		static void CreateVertexBuffer(uint32_t& vbo, float* vertex_array, const int size) 
			{ RenderAPI::s_instance->CreateVertexBuffer(vbo, vertex_array, size); }
		static void CreateVertexAttrib(uint32_t& vao)
			{ RenderAPI::s_instance->CreateVertexAttrib(vao); }
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