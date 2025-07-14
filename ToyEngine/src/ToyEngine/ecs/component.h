#pragma once
#include "ToyEngine/core.h"
#include <typeindex>

namespace ToyEngine
{
	/// <summary>
	/// Base interface for all components in the ECS system.
	/// Components are data containers that define the properties and behavior of entities.
	/// </summary>
	class Component
	{
	public:
		Component() = default;
		virtual ~Component() = default;

		// Delete copy constructor and assignment operator to prevent slicing
		Component(const Component&) = delete;
		Component& operator=(const Component&) = delete;

		// Allow move semantics
		Component(Component&&) = default;
		Component& operator=(Component&&) = default;

		/// <summary>
		/// Gets the type index of this component, used for component identification
		/// </summary>
		/// <returns>Type index of the component</returns>
		virtual std::type_index GetTypeIndex() const = 0;

		/// <summary>
		/// Gets a unique identifier for this component type
		/// </summary>
		/// <returns>Component type ID</returns>
		virtual uint32_t GetComponentTypeId() const = 0;

		/// <summary>
		/// Virtual update method that can be overridden by specific components
		/// </summary>
		/// <param name="deltaTime">Time elapsed since last update</param>
		virtual void Update(float deltaTime) {}

		/// <summary>
		/// Virtual initialization method called when component is attached to entity
		/// </summary>
		virtual void OnAttach() {}

		/// <summary>
		/// Virtual cleanup method called when component is detached from entity
		/// </summary>
		virtual void OnDetach() {}

		/// <summary>
		/// Check if this component is enabled
		/// </summary>
		/// <returns>True if component is enabled</returns>
		inline bool IsEnabled() const { return enabled_; }

		/// <summary>
		/// Enable or disable this component
		/// </summary>
		/// <param name="enabled">Whether to enable the component</param>
		inline void SetEnabled(bool enabled) { enabled_ = enabled; }

	private:
		bool enabled_ = true;
	};

	/// <summary>
	/// Template base class for typed components. Provides automatic type information.
	/// </summary>
	template<typename T>
	class TypedComponent : public Component
	{
	public:
		std::type_index GetTypeIndex() const override
		{
			return std::type_index(typeid(T));
		}

		uint32_t GetComponentTypeId() const override
		{
			return GetStaticComponentTypeId();
		}

		/// <summary>
		/// Gets the static type ID for this component type
		/// </summary>
		/// <returns>Static component type ID</returns>
		static uint32_t GetStaticComponentTypeId()
		{
			static uint32_t type_id = GenerateComponentTypeId();
			return type_id;
		}

	private:
		static uint32_t GenerateComponentTypeId()
		{
			static uint32_t counter = 0;
			return ++counter;
		}
	};
}