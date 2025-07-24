#pragma once
#include "ToyEngine/entity.h"
#include "ToyEngine/renderer/texture_cube.h"
#include "ToyEngine/renderer/shader.h"

namespace ToyEngine {
	class Skybox : public Entity
	{
	public:
		Skybox(Ref<TextureCube> texture, Ref<Shader> shader);
		~Skybox();

		virtual void Render(const glm::mat4& world_transform = glm::mat4(1.0f)) const override;
		bool IsValid() const { return s_vao != 0 && s_vbo != 0 && texture_ && m_shader; }


		Ref<Shader> m_shader;
	private:
		Ref<TextureCube> texture_;
		
		static uint32_t s_vao, s_vbo;
		static float s_vertices[108]; 
	};
}
