#include "pch.h"
#include "ToyEngine/ecs/scene.h"
#include "ToyEngine/log.h"
#include <algorithm>

namespace ToyEngine
{
	Scene::Scene(const std::string& name)
		: name_(name)
	{
		TY_CORE_INFO("Created scene '{}'", name_);
	}

	Scene::~Scene()
	{
		TY_CORE_INFO("Destroying scene '{}'", name_);
		Clear();
	}

	Entity& Scene::CreateEntity(const std::string& name)
	{
		auto entity = MakeScope<Entity>(name);
		Entity& entity_ref = *entity;
		
		// Every entity should have a Transform component by default
		entity_ref.AddComponent<Transform>();
		
		entities_.push_back(std::move(entity));
		
		TY_CORE_INFO("Created entity '{}' with ID {} in scene '{}'", name, entity_ref.GetId(), name_);
		
		return entity_ref;
	}

	bool Scene::DestroyEntity(uint32_t entity_id)
	{
		auto it = std::find_if(entities_.begin(), entities_.end(),
			[entity_id](const Scope<Entity>& entity) 
			{
				return entity->GetId() == entity_id;
			});

		if (it != entities_.end())
		{
			// Remove parent-child relationships before destroying
			Entity* entity = it->get();
			
			// Remove from parent if it has one
			if (entity->HasComponent<Transform>())
			{
				Transform& transform = entity->GetComponent<Transform>();
				if (transform.GetParent() != nullptr)
				{
					transform.SetParent(nullptr);
				}
				
				// Remove all children (they become root entities)
				for (Transform* child : transform.GetChildren())
				{
					child->SetParent(nullptr);
				}
			}
			
			TY_CORE_INFO("Destroying entity '{}' with ID {} from scene '{}'", 
				entity->GetName(), entity->GetId(), name_);
			
			entities_.erase(it);
			return true;
		}
		
		return false;
	}

	bool Scene::DestroyEntity(Entity& entity)
	{
		return DestroyEntity(entity.GetId());
	}

	Entity* Scene::GetEntity(uint32_t entity_id)
	{
		auto it = std::find_if(entities_.begin(), entities_.end(),
			[entity_id](const Scope<Entity>& entity) 
			{
				return entity->GetId() == entity_id;
			});

		return (it != entities_.end()) ? it->get() : nullptr;
	}

	const Entity* Scene::GetEntity(uint32_t entity_id) const
	{
		auto it = std::find_if(entities_.begin(), entities_.end(),
			[entity_id](const Scope<Entity>& entity) 
			{
				return entity->GetId() == entity_id;
			});

		return (it != entities_.end()) ? it->get() : nullptr;
	}

	Entity* Scene::GetEntity(const std::string& name)
	{
		auto it = std::find_if(entities_.begin(), entities_.end(),
			[&name](const Scope<Entity>& entity) 
			{
				return entity->GetName() == name;
			});

		return (it != entities_.end()) ? it->get() : nullptr;
	}

	const Entity* Scene::GetEntity(const std::string& name) const
	{
		auto it = std::find_if(entities_.begin(), entities_.end(),
			[&name](const Scope<Entity>& entity) 
			{
				return entity->GetName() == name;
			});

		return (it != entities_.end()) ? it->get() : nullptr;
	}

	Vector<Entity*> Scene::GetAllEntities()
	{
		Vector<Entity*> result;
		result.reserve(entities_.size());
		
		for (auto& entity : entities_)
		{
			result.push_back(entity.get());
		}
		
		return result;
	}

	Vector<const Entity*> Scene::GetAllEntities() const
	{
		Vector<const Entity*> result;
		result.reserve(entities_.size());
		
		for (const auto& entity : entities_)
		{
			result.push_back(entity.get());
		}
		
		return result;
	}

	void Scene::Update(float deltaTime)
	{
		// Update all active entities
		for (auto& entity : entities_)
		{
			if (entity->IsActive())
			{
				entity->Update(deltaTime);
			}
		}
	}

	void Scene::Clear()
	{
		TY_CORE_INFO("Clearing all entities from scene '{}'", name_);
		entities_.clear();
	}

	bool Scene::SetEntityParent(Entity& parent, Entity& child)
	{
		// Both entities must have Transform components
		if (!parent.HasComponent<Transform>() || !child.HasComponent<Transform>())
		{
			TY_CORE_ERROR("Cannot set parent-child relationship: entities must have Transform components");
			return false;
		}

		// Prevent circular dependencies
		Transform& parent_transform = parent.GetComponent<Transform>();
		Transform& child_transform = child.GetComponent<Transform>();
		
		// Check if parent is already a child of child (would create circular reference)
		Transform* current = &parent_transform;
		while (current != nullptr)
		{
			if (current == &child_transform)
			{
				TY_CORE_ERROR("Cannot set parent-child relationship: would create circular dependency");
				return false;
			}
			current = current->GetParent();
		}

		// Set parent-child relationship
		child_transform.SetParent(&parent_transform);
		
		TY_CORE_INFO("Set entity '{}' as parent of entity '{}'", parent.GetName(), child.GetName());
		return true;
	}

	bool Scene::RemoveEntityParent(Entity& entity)
	{
		if (!entity.HasComponent<Transform>())
		{
			return false;
		}

		Transform& transform = entity.GetComponent<Transform>();
		if (transform.GetParent() != nullptr)
		{
			TY_CORE_INFO("Removing parent from entity '{}'", entity.GetName());
			transform.SetParent(nullptr);
			return true;
		}
		
		return false;
	}

	Vector<Entity*> Scene::GetRootEntities()
	{
		Vector<Entity*> result;
		
		for (auto& entity : entities_)
		{
			if (entity->HasComponent<Transform>())
			{
				Transform& transform = entity->GetComponent<Transform>();
				if (transform.GetParent() == nullptr)
				{
					result.push_back(entity.get());
				}
			}
		}
		
		return result;
	}

	Vector<const Entity*> Scene::GetRootEntities() const
	{
		Vector<const Entity*> result;
		
		for (const auto& entity : entities_)
		{
			if (entity->HasComponent<Transform>())
			{
				const Transform& transform = entity->GetComponent<Transform>();
				if (transform.GetParent() == nullptr)
				{
					result.push_back(entity.get());
				}
			}
		}
		
		return result;
	}
}