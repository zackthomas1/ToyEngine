#pragma once
#include "ToyEngine/renderer/shader_s.h"
#include "ToyEngine/renderer/mesh/mesh.h"
#include "ToyEngine/renderer/material.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ToyEngine
{
	class Model
	{
	public:
		Model() {}
		Model(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material) :
			shader_(shader), mesh_(mesh), material_(material) {}
		virtual ~Model() {}

		void SetMaterialUniforms();

		std::shared_ptr<Shader> GetShader() const { return shader_; }
		std::shared_ptr<Mesh> GetMesh() const { return mesh_; }
		std::shared_ptr<Material> GetMaterial() const { return material_; }

	private: 
		glm::mat4 model_mat_;
		std::shared_ptr<Shader> shader_;
		std::shared_ptr<Mesh> mesh_;
		std::shared_ptr<Material> material_;
	};
}