#pragma once
#include "ToyEngine/core.h"
#include "ToyEngine/ecs/scene.h"
#include "ToyEngine/renderer/camera.h"
#include "ToyEngine/renderer/renderer.h"

namespace ToyEngine
{
	/// <summary>
	/// SceneRenderer integrates the ECS system with the existing ToyEngine renderer.
	/// It handles rendering all entities with Renderable components in a scene.
	/// </summary>
	class SceneRenderer
	{
	public:
		/// <summary>
		/// Renders all renderable entities in the scene
		/// </summary>
		/// <param name="scene">Scene to render</param>
		/// <param name="camera">Camera to render from</param>
		static void RenderScene(Scene& scene, Ref<Camera> camera);

		/// <summary>
		/// Renders all renderable entities in the scene with a specific render layer
		/// </summary>
		/// <param name="scene">Scene to render</param>
		/// <param name="camera">Camera to render from</param>
		/// <param name="layer">Render layer to render</param>
		static void RenderSceneLayer(Scene& scene, Ref<Camera> camera, int layer);

		/// <summary>
		/// Renders all renderable entities in the scene within a range of render layers
		/// </summary>
		/// <param name="scene">Scene to render</param>
		/// <param name="camera">Camera to render from</param>
		/// <param name="min_layer">Minimum render layer</param>
		/// <param name="max_layer">Maximum render layer</param>
		static void RenderSceneLayerRange(Scene& scene, Ref<Camera> camera, int min_layer, int max_layer);

		/// <summary>
		/// Renders a specific entity if it has a Renderable component
		/// </summary>
		/// <param name="entity">Entity to render</param>
		static void RenderEntity(Entity& entity);

		/// <summary>
		/// Updates the model matrices for all entities with Transform and Renderable components
		/// This should be called before rendering to ensure transforms are up to date
		/// </summary>
		/// <param name="scene">Scene to update</param>
		static void UpdateEntityTransforms(Scene& scene);

		/// <summary>
		/// Gets all renderable entities in the scene, sorted by render layer
		/// </summary>
		/// <param name="scene">Scene to get renderables from</param>
		/// <returns>Vector of entities with Renderable components, sorted by render layer</returns>
		static Vector<Entity*> GetSortedRenderableEntities(Scene& scene);

		/// <summary>
		/// Enables/disables frustum culling for the scene renderer
		/// </summary>
		/// <param name="enabled">Whether to enable frustum culling</param>
		static void SetFrustumCullingEnabled(bool enabled) { frustum_culling_enabled_ = enabled; }

		/// <summary>
		/// Gets whether frustum culling is enabled
		/// </summary>
		/// <returns>True if frustum culling is enabled</returns>
		static bool IsFrustumCullingEnabled() { return frustum_culling_enabled_; }

	private:
		static bool frustum_culling_enabled_;
		
		/// <summary>
		/// Checks if an entity should be rendered based on various criteria
		/// </summary>
		/// <param name="entity">Entity to check</param>
		/// <returns>True if entity should be rendered</returns>
		static bool ShouldRenderEntity(Entity& entity);

		/// <summary>
		/// Compares two entities for render order sorting
		/// </summary>
		/// <param name="a">First entity</param>
		/// <param name="b">Second entity</param>
		/// <returns>True if a should be rendered before b</returns>
		static bool CompareRenderOrder(Entity* a, Entity* b);
	};
}