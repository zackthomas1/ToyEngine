#pragma once
#include "ToyEngine/enum.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define MAX_LIGHTS 12

namespace ToyEngine
{
	struct Light {
	public:
		Light(eLightType type = eLightType::kPoint) : m_type(int(type)), m_enabled(false),
			m_value(glm::vec4(1.0f)),
			m_direction(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)),
			m_position(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)),
			m_spotDirection(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)),
			m_innerAngle(glm::cos(glm::radians(12.5f))), m_outerAngle(glm::cos(glm::radians(20.0f)))
		{}

		//General light parameters
		int m_type;
		int m_enabled;
		float m_innerAngle, m_outerAngle;

		glm::vec4 m_value;
		glm::vec4 m_direction;
		glm::vec4 m_position;
		glm::vec4 m_spotDirection;

		// NOTE: The inner and outer angles are measured in terms of their cosine value.
	};

	struct LightBlock {
		Light m_lights[MAX_LIGHTS];
		uint32_t m_num_lights;
	private: 
		int padding_[3];
	};
}
