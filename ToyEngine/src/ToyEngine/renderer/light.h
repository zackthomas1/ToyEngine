#pragma once
#include "ToyEngine/enum.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ToyEngine
{
	class Light {
	public:
		Light(eLightType type) : m_type(type), m_enabled(true),
			m_ambient(glm::vec3(1.0f) * 0.2f),
			m_diffuse(glm::vec3(1.0f)),
			m_specular(glm::vec3(1.0f) * 0.5f), 
			{}
		virtual ~Light(){}

		//General light parameters
		eLightType m_type;
		bool m_enabled;
		glm::vec3 m_ambient, m_diffuse, m_specular;

		// Directional light parameters

		// Point light parameters

		// Spot light parameters
	};

	class DirectionalLight : public Light
	{
	public:
		DirectionalLight(eLightType type) : Light(type),
			m_direction(glm::vec3(0.0f, 0.0f, -1.0f)) {}

		glm::vec3 m_direction;
	};

	struct PointLight : public Light {
	public:
		PointLight(eLightType type) : Light(type),
			m_position(glm::vec3(0.0f, 0.0f, -1.0f)), 
			m_constant(1.0f), m_linear(1.0f), m_quadratic(1.0f) {}

		glm::vec3 m_position;
		float m_constant, m_linear, m_quadratic;
	};

	struct SpotLight : public Light {
	public:
		SpotLight(eLightType type) : Light(type),
			m_position(glm::vec3(0.0f, 0.0f, -1.0f)), m_spotDirection(glm::vec3(0.0f, 0.0f, -1.0f)),
			m_constant(1.0f), m_linear(0.09f), m_quadratic(0.032f),
			m_innerAngle(15.0f), m_outerAngle(30.0f) {}

		glm::vec3 m_position, m_spotDirection;
		float m_constant, m_linear, m_quadratic;
		float m_innerAngle, m_outerAngle;
	};
}
