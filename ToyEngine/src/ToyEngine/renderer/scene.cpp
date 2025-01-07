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
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
	};

	Scene::Scene()
	{
		// Create scene cameras
		cameras_.push_back(new FlyCamera());
		cameras_.push_back(new OrthographicCamera());

		// Create scene geometry
		std::shared_ptr<CubeMesh> mesh = std::make_shared<CubeMesh>();
		std::shared_ptr<Material> material = std::make_shared<Material>();

		for (int i = 0; i < sizeof(cube_positions) / sizeof(glm::vec3); i++)
		{
			std::shared_ptr<Model> cube = std::make_shared<Model>(mesh, material);

			glm::mat4 transforms = glm::mat4(1.0f);
			transforms = glm::translate(transforms, cube_positions[i]);
			transforms = glm::rotate(transforms, 20.0f * i, glm::vec3(1.0f, 0.3f, 0.5f));
			cube->SetModelMatrix(transforms);

			models_.push_back(cube);
		}
	}

	Scene::~Scene()
	{
		for (Camera* camera : cameras_) 
		{
			delete camera;
		}
	}

	void Scene::Update(float time_delta)
	{
		for(int i = 0; i < GetModels().size(); i++)
		{
			std::shared_ptr<Model> model = GetModels()[i];
			glm::mat4 transforms = model->GetModelMatrix();
			
			if (i % 2 == 0)
				transforms = glm::rotate(transforms, glm::radians(180.0f * time_delta), glm::vec3(1.0f, 0.3f * i, 0.5f * i));
			
			model->SetModelMatrix(transforms);
		}
	}

	void Scene::AddModel(std::shared_ptr<Model> model)
	{
		models_.push_back(model);
	}
}
