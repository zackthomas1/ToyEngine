#include "pch.h"
#include "ToyEngine/ecs/scene_renderer.h"
#include "ToyEngine/ecs/transform.h"
#include "ToyEngine/ecs/renderable.h"
#include "ToyEngine/log.h"
#include <algorithm>

namespace ToyEngine
{
	bool SceneRenderer::frustum_culling_enabled_ = false;

	void SceneRenderer::RenderScene(Scene& scene, Ref<Camera> camera)
	{
		// Update entity transforms before rendering
		UpdateEntityTransforms(scene);

		// Begin scene rendering
		Renderer::BeginScene(camera);

		// Get all renderable entities sorted by render layer
		Vector<Entity*> renderables = GetSortedRenderableEntities(scene);

		// Render each entity
		for (Entity* entity : renderables)
		{
			if (ShouldRenderEntity(*entity))
			{
				RenderEntity(*entity);
			}
		}

		// End scene rendering
		Renderer::EndScene();
	}

	void SceneRenderer::RenderSceneLayer(Scene& scene, Ref<Camera> camera, int layer)
	{
		UpdateEntityTransforms(scene);
		Renderer::BeginScene(camera);

		Vector<Entity*> renderables = scene.GetEntitiesWithComponent<Renderable>();

		for (Entity* entity : renderables)
		{
			if (ShouldRenderEntity(*entity))
			{
				Renderable& renderable = entity->GetComponent<Renderable>();
				if (renderable.GetRenderLayer() == layer)
				{
					RenderEntity(*entity);
				}
			}
		}

		Renderer::EndScene();
	}

	void SceneRenderer::RenderSceneLayerRange(Scene& scene, Ref<Camera> camera, int min_layer, int max_layer)
	{
		UpdateEntityTransforms(scene);
		Renderer::BeginScene(camera);

		Vector<Entity*> renderables = scene.GetEntitiesWithComponent<Renderable>();

		for (Entity* entity : renderables)
		{
			if (ShouldRenderEntity(*entity))
			{
				Renderable& renderable = entity->GetComponent<Renderable>();
				int layer = renderable.GetRenderLayer();
				if (layer >= min_layer && layer <= max_layer)
				{
					RenderEntity(*entity);
				}
			}
		}

		Renderer::EndScene();
	}

	void SceneRenderer::RenderEntity(Entity& entity)
	{
		if (!entity.HasComponent<Renderable>())
		{
			return;
		}

		Renderable& renderable = entity.GetComponent<Renderable>();
		
		// Check if renderable is ready for rendering
		if (!renderable.IsReadyForRendering() || !renderable.IsVisible())
		{
			return;
		}

		// Get the model and shader
		Ref<Model> model = renderable.GetModel();
		Ref<Shader> shader = renderable.GetShader();

		// Update model matrix if entity has a Transform component
		if (entity.HasComponent<Transform>())
		{
			Transform& transform = entity.GetComponent<Transform>();
			model->m_model_mat = transform.GetWorldMatrix();
		}

		// Submit to renderer
		Renderer::Submit(shader, model);
	}

	void SceneRenderer::UpdateEntityTransforms(Scene& scene)
	{
		// Get all entities with Transform components
		Vector<Entity*> transform_entities = scene.GetEntitiesWithComponent<Transform>();

		// Update transforms (this will handle dirty checking internally)
		for (Entity* entity : transform_entities)
		{
			Transform& transform = entity->GetComponent<Transform>();
			// Just access the world matrix to trigger updates if needed
			transform.GetWorldMatrix();
		}
	}

	Vector<Entity*> SceneRenderer::GetSortedRenderableEntities(Scene& scene)
	{
		Vector<Entity*> renderables = scene.GetEntitiesWithComponent<Renderable>();
		
		// Sort by render layer
		std::sort(renderables.begin(), renderables.end(), CompareRenderOrder);
		
		return renderables;
	}

	bool SceneRenderer::ShouldRenderEntity(Entity& entity)
	{
		// Check if entity is active
		if (!entity.IsActive())
		{
			return false;
		}

		// Check if entity has a renderable component
		if (!entity.HasComponent<Renderable>())
		{
			return false;
		}

		Renderable& renderable = entity.GetComponent<Renderable>();
		
		// Check if renderable is visible and ready
		if (!renderable.IsVisible() || !renderable.IsReadyForRendering())
		{
			return false;
		}

		// TODO: Add frustum culling here if enabled
		// if (frustum_culling_enabled_)
		// {
		//     // Perform frustum culling check
		// }

		return true;
	}

	bool SceneRenderer::CompareRenderOrder(Entity* a, Entity* b)
	{
		if (!a->HasComponent<Renderable>() || !b->HasComponent<Renderable>())
		{
			return false;
		}

		Renderable& renderable_a = a->GetComponent<Renderable>();
		Renderable& renderable_b = b->GetComponent<Renderable>();

		// Sort by render layer (lower layers rendered first)
		return renderable_a.GetRenderLayer() < renderable_b.GetRenderLayer();
	}
}