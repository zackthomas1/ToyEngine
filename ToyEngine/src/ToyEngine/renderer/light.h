#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

namespace ToyEngine
{
	// Light type enumeration
	enum class LightType
	{
		Directional,
		Point,
		Spot
	};

	// Base light structure with common properties
	struct Light
	{
		LightType type;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		bool enabled = true;

		Light(LightType lightType) : type(lightType) {}
		virtual ~Light() = default;
	};

	// Directional light - no position, only direction
	struct DirectionalLight : public Light
	{
		glm::vec3 direction;

		DirectionalLight() : Light(LightType::Directional)
		{
			direction = glm::vec3(0.0f, -1.0f, 0.0f);
			ambient = glm::vec3(0.2f, 0.2f, 0.2f);
			diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
			specular = glm::vec3(0.5f, 0.5f, 0.5f);
		}
	};

	// Point light - position with attenuation
	struct PointLight : public Light
	{
		glm::vec3 position;
		float constant;
		float linear;
		float quadratic;

		PointLight() : Light(LightType::Point)
		{
			position = glm::vec3(0.0f, 0.0f, 0.0f);
			ambient = glm::vec3(0.1f, 0.1f, 0.1f);
			diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
			specular = glm::vec3(1.0f, 1.0f, 1.0f);
			constant = 1.0f;
			linear = 0.09f;
			quadratic = 0.032f;
		}
	};

	// Spot light - position, direction, and cone angles
	struct SpotLight : public Light
	{
		glm::vec3 position;
		glm::vec3 direction;
		float innerCone;  // In radians
		float outerCone;  // In radians
		float constant;
		float linear;
		float quadratic;

		SpotLight() : Light(LightType::Spot)
		{
			position = glm::vec3(0.0f, 0.0f, 0.0f);
			direction = glm::vec3(0.0f, -1.0f, 0.0f);
			ambient = glm::vec3(0.1f, 0.1f, 0.1f);
			diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
			specular = glm::vec3(1.0f, 1.0f, 1.0f);
			innerCone = glm::cos(glm::radians(12.5f));
			outerCone = glm::cos(glm::radians(17.5f));
			constant = 1.0f;
			linear = 0.09f;
			quadratic = 0.032f;
		}
	};
}
