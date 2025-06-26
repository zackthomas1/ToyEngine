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
		Model() : model_mat_() {}
		Model(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material) :
			model_mat_(glm::mat4(1.0f)), mesh_(mesh), material_(material) {}
		virtual ~Model() {}

		// getter/setter
		glm::mat4 GetModelMatrix() const { return model_mat_; }
		std::shared_ptr<Mesh> GetMesh() const { return mesh_; }
		std::shared_ptr<Material> GetMaterial() const { return material_; }

		void SetModelMatrix(glm::mat4 transformation) { model_mat_ = transformation; }

	private: 
		glm::mat4 model_mat_;
		std::shared_ptr<Mesh> mesh_;
		std::shared_ptr<Material> material_;
	};
}