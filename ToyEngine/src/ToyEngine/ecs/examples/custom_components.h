#pragma once
#include "ToyEngine/ecs/component.h"
#include <glm/glm.hpp>

namespace ToyEngine
{
	/// <summary>
	/// Example custom component that demonstrates how to extend the ECS system.
	/// This component makes entities move in a circular pattern.
	/// </summary>
	class CircularMovement : public TypedComponent<CircularMovement>
	{
	public:
		CircularMovement(float radius = 1.0f, float speed = 1.0f)
			: radius_(radius), speed_(speed)
		{
		}

		void Update(float deltaTime) override
		{
			if (!IsEnabled())
				return;

			time_ += deltaTime * speed_;
			
			// Calculate new position in circular pattern
			float x = radius_ * cos(time_);
			float z = radius_ * sin(time_);
			
			calculated_offset_ = glm::vec3(x, 0.0f, z);
		}

		/// <summary>
		/// Gets the calculated offset for circular movement
		/// </summary>
		/// <returns>Offset vector</returns>
		glm::vec3 GetOffset() const { return calculated_offset_; }

		/// <summary>
		/// Gets the radius of the circular movement
		/// </summary>
		/// <returns>Radius</returns>
		float GetRadius() const { return radius_; }

		/// <summary>
		/// Sets the radius of the circular movement
		/// </summary>
		/// <param name="radius">New radius</param>
		void SetRadius(float radius) { radius_ = radius; }

		/// <summary>
		/// Gets the speed of the circular movement
		/// </summary>
		/// <returns>Speed multiplier</returns>
		float GetSpeed() const { return speed_; }

		/// <summary>
		/// Sets the speed of the circular movement
		/// </summary>
		/// <param name="speed">New speed multiplier</param>
		void SetSpeed(float speed) { speed_ = speed; }

		/// <summary>
		/// Resets the time to start from the beginning
		/// </summary>
		void Reset() { time_ = 0.0f; }

	private:
		float radius_;
		float speed_;
		float time_ = 0.0f;
		glm::vec3 calculated_offset_ = glm::vec3(0.0f);
	};

	/// <summary>
	/// Example component that demonstrates component lifecycle and entity interaction
	/// </summary>
	class LifecycleExample : public TypedComponent<LifecycleExample>
	{
	public:
		LifecycleExample(const std::string& name = "LifecycleExample")
			: name_(name)
		{
		}

		void OnAttach() override
		{
			TY_INFO("Component '{}' attached to entity", name_);
		}

		void OnDetach() override
		{
			TY_INFO("Component '{}' detached from entity", name_);
		}

		void Update(float deltaTime) override
		{
			if (!IsEnabled())
				return;

			lifetime_ += deltaTime;
			
			// Example: disable component after 10 seconds
			if (lifetime_ > 10.0f)
			{
				SetEnabled(false);
				TY_INFO("Component '{}' disabled after {} seconds", name_, lifetime_);
			}
		}

		float GetLifetime() const { return lifetime_; }
		const std::string& GetName() const { return name_; }

	private:
		std::string name_;
		float lifetime_ = 0.0f;
	};
}