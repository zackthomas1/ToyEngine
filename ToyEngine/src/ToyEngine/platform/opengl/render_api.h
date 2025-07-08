#pragma once
#include "ToyEngine/platform/opengl/shader_s.h"
#include "ToyEngine/renderer/model.h"

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

		static void CreateVertexArray(unsigned int* vao);
		static void BindVertexArray(unsigned int vao);
		static void DeleteVertexArray(unsigned int id);
		
		static void CreateVertexBuffer(unsigned int* vbo, float* vertex_array, const int size);
		static void CreateVertexAttrib(unsigned int* vao);
		static void DeleteBuffer(unsigned int id);
		static void DrawArrays(unsigned int vertices);
		static void DrawIndexed(unsigned int vao);

		static void PolygonMode(uint32_t face, uint32_t mode);
	};
}