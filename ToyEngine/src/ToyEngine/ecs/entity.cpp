#include "pch.h"
#include "ToyEngine/ecs/entity.h"
#include "ToyEngine/log.h"

namespace ToyEngine
{
	uint32_t Entity::s_next_id_ = 1;

	Entity::Entity(const std::string& name)
		: id_(s_next_id_++), name_(name)
	{
		TY_CORE_INFO("Created entity '{}' with ID {}", name_, id_);
	}

	Entity::~Entity()
	{
		TY_CORE_INFO("Destroying entity '{}' with ID {}", name_, id_);
		
		// OnDetach will be called for all components when they're destroyed
		for (auto& [type_idx, component] : components_)
		{
			component->OnDetach();
		}
		
		components_.clear();
	}

	void Entity::Update(float deltaTime)
	{
		if (!active_)
			return;

		// Update all enabled components
		for (auto& [type_idx, component] : components_)
		{
			if (component->IsEnabled())
			{
				component->Update(deltaTime);
			}
		}
	}
}