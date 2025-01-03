#include "pch.h"
#include "scene.h"

#include "ToyEngine/renderer/mesh/primatives/cube_mesh.h"
#include "ToyEngine/renderer/mesh/primatives/triangle_mesh.h"
#include "ToyEngine/renderer/mesh/primatives/plane_mesh.h"

#include "ToyEngine/renderer/camera/fly_camera.h"
#include "ToyEngine/renderer/camera/orthographic_camera.h"

namespace ToyEngine
{
	// TODO: Remove cube position array.
	// This is for testing
	glm::vec3 cube_positions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	Scene::Scene()
	{
		// Create scene cameras
		cameras_.push_back(std::make_shared<FlyCamera>());
		cameras_.push_back(std::make_shared<OrthographicCamera>());

		// Create scene geometry
		std::shared_ptr<CubeMesh> mesh = std::make_shared<CubeMesh>();
		std::shared_ptr<Material> material = std::make_shared<Material>();

		for (int i = 0; i < sizeof(cube_positions) / sizeof(glm::vec3); i++)
		{
			std::shared_ptr<Model> cube = std::make_shared<Model>(mesh, material);

			glm::mat4 transforms = glm::mat4(1.0f);
			transforms = glm::translate(transforms, cube_positions[i]);
			cube->SetModelMatrix(transforms);

			models_.push_back(cube);
		}
	}

	Scene::~Scene()
	{
	}

	void Scene::Update(float time_delta)
	{
	}

	void Scene::AddModel(std::shared_ptr<Model> model)
	{
		models_.push_back(model);
	}
}
