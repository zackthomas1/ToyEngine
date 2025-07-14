#pragma once
#include "ToyEngine/core.h"
#include "ToyEngine/ecs/component.h"
#include <unordered_map>
#include <typeindex>
#include <string>

namespace ToyEngine
{
	/// <summary>
	/// Entity represents a game object in the ECS system.
	/// Entities are containers for components and don't have behavior themselves.
	/// </summary>
	class Entity
	{
	public:
		/// <summary>
		/// Creates a new entity with a unique ID
		/// </summary>
		/// <param name="name">Optional name for the entity</param>
		Entity(const std::string& name = "Entity");

		/// <summary>
		/// Destructor - ensures proper cleanup of components
		/// </summary>
		~Entity();

		// Delete copy constructor and assignment operator to prevent issues with unique IDs
		Entity(const Entity&) = delete;
		Entity& operator=(const Entity&) = delete;

		// Allow move semantics
		Entity(Entity&&) = default;
		Entity& operator=(Entity&&) = default;

		/// <summary>
		/// Adds a component to this entity
		/// </summary>
		/// <typeparam name="T">Component type</typeparam>
		/// <typeparam name="...Args">Constructor arguments</typeparam>
		/// <param name="...args">Arguments to pass to component constructor</param>
		/// <returns>Reference to the created component</returns>
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of_v<Component, T>, "T must be derived from Component");
			
			std::type_index type_idx(typeid(T));
			
			// Remove existing component of same type if it exists
			if (HasComponent<T>())
			{
				RemoveComponent<T>();
			}

			// Create new component
			auto component = MakeScope<T>(std::forward<Args>(args)...);
			T& component_ref = *component;
			
			// Store component
			components_[type_idx] = std::move(component);
			
			// Call OnAttach
			component_ref.OnAttach();
			
			return component_ref;
		}

		/// <summary>
		/// Gets a component of the specified type
		/// </summary>
		/// <typeparam name="T">Component type</typeparam>
		/// <returns>Reference to the component</returns>
		template<typename T>
		T& GetComponent()
		{
			static_assert(std::is_base_of_v<Component, T>, "T must be derived from Component");
			
			std::type_index type_idx(typeid(T));
			auto it = components_.find(type_idx);
			
			TY_CORE_ASSERT(it != components_.end(), "Entity does not have component of type {}", typeid(T).name());
			
			return static_cast<T&>(*it->second);
		}

		/// <summary>
		/// Gets a component of the specified type (const version)
		/// </summary>
		/// <typeparam name="T">Component type</typeparam>
		/// <returns>Const reference to the component</returns>
		template<typename T>
		const T& GetComponent() const
		{
			static_assert(std::is_base_of_v<Component, T>, "T must be derived from Component");
			
			std::type_index type_idx(typeid(T));
			auto it = components_.find(type_idx);
			
			TY_CORE_ASSERT(it != components_.end(), "Entity does not have component of type {}", typeid(T).name());
			
			return static_cast<const T&>(*it->second);
		}

		/// <summary>
		/// Tries to get a component of the specified type
		/// </summary>
		/// <typeparam name="T">Component type</typeparam>
		/// <returns>Pointer to the component, or nullptr if not found</returns>
		template<typename T>
		T* TryGetComponent()
		{
			static_assert(std::is_base_of_v<Component, T>, "T must be derived from Component");
			
			std::type_index type_idx(typeid(T));
			auto it = components_.find(type_idx);
			
			if (it != components_.end())
			{
				return static_cast<T*>(it->second.get());
			}
			
			return nullptr;
		}

		/// <summary>
		/// Tries to get a component of the specified type (const version)
		/// </summary>
		/// <typeparam name="T">Component type</typeparam>
		/// <returns>Const pointer to the component, or nullptr if not found</returns>
		template<typename T>
		const T* TryGetComponent() const
		{
			static_assert(std::is_base_of_v<Component, T>, "T must be derived from Component");
			
			std::type_index type_idx(typeid(T));
			auto it = components_.find(type_idx);
			
			if (it != components_.end())
			{
				return static_cast<const T*>(it->second.get());
			}
			
			return nullptr;
		}

		/// <summary>
		/// Checks if the entity has a component of the specified type
		/// </summary>
		/// <typeparam name="T">Component type</typeparam>
		/// <returns>True if the entity has the component</returns>
		template<typename T>
		bool HasComponent() const
		{
			static_assert(std::is_base_of_v<Component, T>, "T must be derived from Component");
			
			std::type_index type_idx(typeid(T));
			return components_.find(type_idx) != components_.end();
		}

		/// <summary>
		/// Removes a component of the specified type
		/// </summary>
		/// <typeparam name="T">Component type</typeparam>
		/// <returns>True if component was removed</returns>
		template<typename T>
		bool RemoveComponent()
		{
			static_assert(std::is_base_of_v<Component, T>, "T must be derived from Component");
			
			std::type_index type_idx(typeid(T));
			auto it = components_.find(type_idx);
			
			if (it != components_.end())
			{
				// Call OnDetach before removing
				it->second->OnDetach();
				components_.erase(it);
				return true;
			}
			
			return false;
		}

		/// <summary>
		/// Updates all components in this entity
		/// </summary>
		/// <param name="deltaTime">Time elapsed since last update</param>
		void Update(float deltaTime);

		/// <summary>
		/// Gets the unique ID of this entity
		/// </summary>
		/// <returns>Entity ID</returns>
		inline uint32_t GetId() const { return id_; }

		/// <summary>
		/// Gets the name of this entity
		/// </summary>
		/// <returns>Entity name</returns>
		inline const std::string& GetName() const { return name_; }

		/// <summary>
		/// Sets the name of this entity
		/// </summary>
		/// <param name="name">New name for the entity</param>
		inline void SetName(const std::string& name) { name_ = name; }

		/// <summary>
		/// Checks if this entity is active
		/// </summary>
		/// <returns>True if entity is active</returns>
		inline bool IsActive() const { return active_; }

		/// <summary>
		/// Sets the active state of this entity
		/// </summary>
		/// <param name="active">Whether the entity should be active</param>
		inline void SetActive(bool active) { active_ = active; }

		/// <summary>
		/// Gets all components in this entity
		/// </summary>
		/// <returns>Map of type indices to components</returns>
		const std::unordered_map<std::type_index, Scope<Component>>& GetComponents() const { return components_; }

	private:
		uint32_t id_;
		std::string name_;
		bool active_ = true;
		std::unordered_map<std::type_index, Scope<Component>> components_;

		// Static counter for generating unique IDs
		static uint32_t s_next_id_;
	};
}