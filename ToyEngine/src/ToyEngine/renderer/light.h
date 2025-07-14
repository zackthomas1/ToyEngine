#pragma once
#include "ToyEngine/enum.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TY_MAX_DIRECTIONAL_LIGHTS 4
#define TY_MAX_POINT_LIGHTS 32
#define TY_MAX_SPOT_LIGHTS 16

namespace ToyEngine
{
	class Light {
	public:
		Light(eLightType type = eLightType::kPoint) : m_type(type), m_enabled(true),
			m_value(glm::vec3(1.0f)),
			m_direction(glm::vec3(0.0f, 0.0f, -1.0f)),
			m_position(glm::vec3(0.0f, 0.0f, -1.0f)),
			m_spotDirection(glm::vec3(0.0f, 0.0f, -1.0f)),
			m_innerAngle(glm::cos(glm::radians(12.5f))), m_outerAngle(glm::cos(glm::radians(20.0f)))
		{}

		//General light parameters
		eLightType m_type;
		bool m_enabled;
		glm::vec3 m_value;

		// Directional light parameters
		glm::vec3 m_direction;

		// Point/Spot light parameters
		glm::vec3 m_position;

		// Spot light parameters
		glm::vec3 m_spotDirection;
		// The inner and outer angles are measured in terms of their cosine value.  
		// Measurements in degrees must first be converted to radians,  
		// and then the cosine of the angle in radians is used.  
		float m_innerAngle, m_outerAngle;
	};
}
