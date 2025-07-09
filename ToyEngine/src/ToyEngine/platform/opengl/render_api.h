#pragma once
#include "ToyEngine/renderer/shader.h"

namespace ToyEngine
{
	class RenderAPI
	{
	public:
		// Enforce static class
		// Delete default, copy, and move constructors
		RenderAPI() = delete;
		RenderAPI(const RenderAPI&) = delete;
		RenderAPI(RenderAPI&&) = delete;

		// Delete the assignment operators
		RenderAPI& operator=(const RenderAPI&) = delete;
		RenderAPI& operator=(RenderAPI&&) = delete;

		// Methods
		static void ClearSetBackground();

		static void ShaderUse(const Shader* shader);

		static void BindTexture2D();
		static void SetTexture2DParameters(uint32_t wrap_s, uint32_t wrap_t, uint32_t min_filter, uint32_t mag_filter);

		static void CreateVertexArray(uint32_t* vao);
		static void BindVertexArray(uint32_t vao);
		static void DeleteVertexArray(uint32_t id);
		
		static void CreateVertexBuffer(uint32_t* vbo, float* vertex_array, const int size);
		static void CreateVertexAttrib(uint32_t* vao);
		static void DeleteBuffer(uint32_t id);
		static void DrawArrays(uint32_t vertices);
		static void DrawIndexed(uint32_t vao);

		static void PolygonMode(uint32_t face, uint32_t mode);
	};
}