#pragma once
#include "ToyEngine/core.h"
#include "ToyEngine/ecs/entity.h"
#include "ToyEngine/ecs/transform.h"
#include <string>

namespace ToyEngine
{
	/// <summary>
	/// Scene manages a collection of entities and provides the scene graph functionality.
	/// This is different from the Scene layer in the editor - this is the ECS scene container.
	/// </summary>
	class Scene
	{
	public:
		/// <summary>
		/// Creates a new scene
		/// </summary>
		/// <param name="name">Name of the scene</param>
		Scene(const std::string& name = "Scene");

		/// <summary>
		/// Destructor
		/// </summary>
		~Scene();

		/// <summary>
		/// Creates a new entity in this scene
		/// </summary>
		/// <param name="name">Name of the entity</param>
		/// <returns>Reference to the created entity</returns>
		Entity& CreateEntity(const std::string& name = "Entity");

		/// <summary>
		/// Destroys an entity by ID
		/// </summary>
		/// <param name="entity_id">ID of the entity to destroy</param>
		/// <returns>True if entity was found and destroyed</returns>
		bool DestroyEntity(uint32_t entity_id);

		/// <summary>
		/// Destroys an entity by reference
		/// </summary>
		/// <param name="entity">Reference to the entity to destroy</param>
		/// <returns>True if entity was found and destroyed</returns>
		bool DestroyEntity(Entity& entity);

		/// <summary>
		/// Gets an entity by ID
		/// </summary>
		/// <param name="entity_id">ID of the entity</param>
		/// <returns>Pointer to the entity, or nullptr if not found</returns>
		Entity* GetEntity(uint32_t entity_id);

		/// <summary>
		/// Gets an entity by ID (const version)
		/// </summary>
		/// <param name="entity_id">ID of the entity</param>
		/// <returns>Const pointer to the entity, or nullptr if not found</returns>
		const Entity* GetEntity(uint32_t entity_id) const;

		/// <summary>
		/// Gets an entity by name (returns first match)
		/// </summary>
		/// <param name="name">Name of the entity</param>
		/// <returns>Pointer to the entity, or nullptr if not found</returns>
		Entity* GetEntity(const std::string& name);

		/// <summary>
		/// Gets an entity by name (const version)
		/// </summary>
		/// <param name="name">Name of the entity</param>
		/// <returns>Const pointer to the entity, or nullptr if not found</returns>
		const Entity* GetEntity(const std::string& name) const;

		/// <summary>
		/// Gets all entities with a specific component type
		/// </summary>
		/// <typeparam name="T">Component type</typeparam>
		/// <returns>Vector of entities with the specified component</returns>
		template<typename T>
		Vector<Entity*> GetEntitiesWithComponent()
		{
			Vector<Entity*> result;
			for (auto& entity : entities_)
			{
				if (entity->HasComponent<T>())
				{
					result.push_back(entity.get());
				}
			}
			return result;
		}

		/// <summary>
		/// Gets all entities with a specific component type (const version)
		/// </summary>
		/// <typeparam name="T">Component type</typeparam>
		/// <returns>Vector of const entities with the specified component</returns>
		template<typename T>
		Vector<const Entity*> GetEntitiesWithComponent() const
		{
			Vector<const Entity*> result;
			for (const auto& entity : entities_)
			{
				if (entity->HasComponent<T>())
				{
					result.push_back(entity.get());
				}
			}
			return result;
		}

		/// <summary>
		/// Gets all entities in the scene
		/// </summary>
		/// <returns>Vector of all entities</returns>
		Vector<Entity*> GetAllEntities();

		/// <summary>
		/// Gets all entities in the scene (const version)
		/// </summary>
		/// <returns>Vector of all entities</returns>
		Vector<const Entity*> GetAllEntities() const;

		/// <summary>
		/// Updates all entities in the scene
		/// </summary>
		/// <param name="deltaTime">Time elapsed since last update</param>
		void Update(float deltaTime);

		/// <summary>
		/// Gets the scene name
		/// </summary>
		/// <returns>Scene name</returns>
		const std::string& GetName() const { return name_; }

		/// <summary>
		/// Sets the scene name
		/// </summary>
		/// <param name="name">New scene name</param>
		void SetName(const std::string& name) { name_ = name; }

		/// <summary>
		/// Gets the number of entities in the scene
		/// </summary>
		/// <returns>Number of entities</returns>
		size_t GetEntityCount() const { return entities_.size(); }

		/// <summary>
		/// Clears all entities from the scene
		/// </summary>
		void Clear();

		/// <summary>
		/// Sets up parent-child relationships between entities using their Transform components
		/// </summary>
		/// <param name="parent">Parent entity</param>
		/// <param name="child">Child entity</param>
		/// <returns>True if relationship was established successfully</returns>
		bool SetEntityParent(Entity& parent, Entity& child);

		/// <summary>
		/// Removes parent-child relationship for an entity
		/// </summary>
		/// <param name="entity">Entity to remove from parent</param>
		/// <returns>True if entity had a parent and was removed</returns>
		bool RemoveEntityParent(Entity& entity);

		/// <summary>
		/// Gets all root entities (entities without parents)
		/// </summary>
		/// <returns>Vector of root entities</returns>
		Vector<Entity*> GetRootEntities();

		/// <summary>
		/// Gets all root entities (entities without parents) const version
		/// </summary>
		/// <returns>Vector of root entities</returns>
		Vector<const Entity*> GetRootEntities() const;

	private:
		std::string name_;
		Vector<Scope<Entity>> entities_;
		uint32_t next_entity_index_ = 0;
	};
}