#include "pch.h"
#include "skybox.h"
#include "renderer/renderer.h"
#include "ToyEngine/renderer/render_api.h"

namespace ToyEngine
{
	float Skybox::s_vertices[108] = {
		// positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	uint32_t Skybox::s_vao = 0;
	uint32_t Skybox::s_vbo = 0;
	int Skybox::s_ref_count = 0;

	Skybox::Skybox(Ref<TextureCube> texture, Ref<Shader> shader) : texture_(texture), m_shader(shader)
	{
		TY_CORE_ASSERT(texture_, "Skybox - Null TextureCube provided");
		TY_CORE_ASSERT(shader, "Skybox - Null Shader provided");

		s_ref_count++;

		if (s_vao == 0 || s_vbo == 0) {
			RenderCommand::GenVertexArrays(1, s_vao);
			RenderCommand::GenBuffers(1, s_vbo);
			RenderCommand::BindVertexArray(s_vao);
			RenderCommand::BindBuffer(eBufferType::kARRAY_BUFFER, s_vbo); 
			RenderCommand::BufferData(eBufferType::kARRAY_BUFFER, sizeof(s_vertices), &s_vertices[0]);
		
			// aPos
			RenderCommand::EnableVertexAttribArray(0);
			RenderCommand::VertexAttribPointer(0,3, eDataType::kFLOAT, 3 * sizeof(float), 0);
			
			// Set binding back to default
			RenderCommand::BindVertexArray(0);
		}
	}

	Skybox::~Skybox()
	{
		s_ref_count--; 
		if(s_ref_count == 0){
			if (s_vao != 0) {
				RenderCommand::DeleteVertexArray(s_vao);
				s_vao = 0;
			}
			if (s_vbo != 0) {
				RenderCommand::DeleteBuffer(s_vbo);
				s_vbo = 0;
			}
		}
	}

	void Skybox::Render(const glm::mat4& world_transform) const
	{
		// validation
		TY_CORE_ASSERT(m_shader, "Skybox - Null Shader");
		TY_CORE_ASSERT(texture_, "Skybox - Null TextureCube");
		TY_CORE_ASSERT(s_vao != 0, "Skybox - Invalid VAO");

		// Save current OpenGL state
		bool depthMask;
		RenderCommand::GetBooleanv(eParamType::kDEPTH_WRITEMASK, &depthMask);
		bool cullFace;
		RenderCommand::GetBooleanv(eParamType::kCULL_FACE, &cullFace);
		int depthFunc;
		RenderCommand::GetIntegerv(eParamType::kDEPTH_FUNC, &depthFunc);

		// Set skybox rendering state
		RenderCommand::DepthMask(false);
		RenderCommand::Disable(eParamType::kCULL_FACE);
		RenderCommand::DepthFunc(eDepthFunc::kLEQUAL);

		m_shader->Use();
		m_shader->SetInt("skybox_texture", 0);
		RenderCommand::BindVertexArray(s_vao);
		texture_->Bind(0);
		RenderCommand::DrawArrays(ePrimType::kTRIANGLE, 0, 36);
		RenderCommand::BindVertexArray(0);

		// Restore previous OpenGL state
		RenderCommand::DepthMask(depthMask);
		if (cullFace) RenderCommand::Enable(eParamType::kCULL_FACE);
		RenderCommand::DepthFunc((eDepthFunc)depthFunc);
	}
}