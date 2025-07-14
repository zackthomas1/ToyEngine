#pragma once
#include "ToyEngine/ecs/scene.h"
#include "ToyEngine/ecs/transform.h"
#include "ToyEngine/ecs/examples/custom_components.h"

namespace ToyEngine
{
	/// <summary>
	/// Example of a system that operates on entities with specific components.
	/// This system applies circular movement to entities that have both CircularMovement and Transform components.
	/// </summary>
	class MovementSystem
	{
	public:
		/// <summary>
		/// Updates all entities in the scene that have CircularMovement components
		/// </summary>
		/// <param name="scene">Scene to update</param>
		/// <param name="deltaTime">Time elapsed since last update</param>
		static void UpdateCircularMovement(Scene& scene, float deltaTime)
		{
			// Get all entities with CircularMovement components
			auto entities = scene.GetEntitiesWithComponent<CircularMovement>();
			
			for (Entity* entity : entities)
			{
				// Only process active entities
				if (!entity->IsActive())
					continue;
				
				// Get the CircularMovement component
				CircularMovement& movement = entity->GetComponent<CircularMovement>();
				
				// Update the component (this calculates the new offset)
				movement.Update(deltaTime);
				
				// Apply the movement to the Transform component if it exists
				if (entity->HasComponent<Transform>())
				{
					Transform& transform = entity->GetComponent<Transform>();
					
					// Get the base position and apply the circular offset
					glm::vec3 base_position = GetBasePosition(*entity);
					glm::vec3 new_position = base_position + movement.GetOffset();
					
					transform.SetLocalPosition(new_position);
				}
			}
		}
		
		/// <summary>
		/// Sets up circular movement for an entity
		/// </summary>
		/// <param name="entity">Entity to add circular movement to</param>
		/// <param name="radius">Radius of the circular movement</param>
		/// <param name="speed">Speed of the circular movement</param>
		/// <param name="base_position">Base position around which to move</param>
		static void AddCircularMovement(Entity& entity, float radius, float speed, const glm::vec3& base_position)
		{
			// Add CircularMovement component
			CircularMovement& movement = entity.AddComponent<CircularMovement>(radius, speed);
			
			// Store the base position (you might want to create a separate component for this)
			// For simplicity, we'll use a tag or store it in the entity name
			entity.SetName(entity.GetName() + "_CircularMovement");
			
			// Set initial position
			if (entity.HasComponent<Transform>())
			{
				Transform& transform = entity.GetComponent<Transform>();
				transform.SetLocalPosition(base_position);
			}
		}
		
		/// <summary>
		/// Removes circular movement from an entity
		/// </summary>
		/// <param name="entity">Entity to remove circular movement from</param>
		static void RemoveCircularMovement(Entity& entity)
		{
			entity.RemoveComponent<CircularMovement>();
		}
		
	private:
		/// <summary>
		/// Gets the base position for an entity (this is a simplified example)
		/// In a real implementation, you might store this in a separate component
		/// </summary>
		/// <param name="entity">Entity to get base position for</param>
		/// <returns>Base position</returns>
		static glm::vec3 GetBasePosition(Entity& entity)
		{
			// For this example, we'll use a simple approach
			// In a real implementation, you might store this in a separate component
			// or calculate it based on the entity's initial position
			return glm::vec3(0.0f, 0.0f, 0.0f);
		}
	};
	
	/// <summary>
	/// Example system that manages component lifecycles
	/// </summary>
	class LifecycleSystem
	{
	public:
		/// <summary>
		/// Updates all lifecycle components and handles their state changes
		/// </summary>
		/// <param name="scene">Scene to update</param>
		/// <param name="deltaTime">Time elapsed since last update</param>
		static void UpdateLifecycles(Scene& scene, float deltaTime)
		{
			auto entities = scene.GetEntitiesWithComponent<LifecycleExample>();
			
			for (Entity* entity : entities)
			{
				if (!entity->IsActive())
					continue;
				
				LifecycleExample& lifecycle = entity->GetComponent<LifecycleExample>();
				
				// Update the component (this handles the lifecycle logic)
				lifecycle.Update(deltaTime);
				
				// Example: Remove component after it's been disabled for 5 seconds
				if (!lifecycle.IsEnabled() && lifecycle.GetLifetime() > 15.0f)
				{
					TY_INFO("Removing expired lifecycle component from entity '{}'", entity->GetName());
					entity->RemoveComponent<LifecycleExample>();
				}
			}
		}
		
		/// <summary>
		/// Adds a lifecycle component to an entity
		/// </summary>
		/// <param name="entity">Entity to add lifecycle to</param>
		/// <param name="name">Name for the lifecycle component</param>
		static void AddLifecycle(Entity& entity, const std::string& name)
		{
			entity.AddComponent<LifecycleExample>(name);
		}
	};
}