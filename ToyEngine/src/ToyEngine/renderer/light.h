#pragma once
#include "ToyEngine/enum.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define MAX_LIGHTS 12

namespace ToyEngine
{
	class Light {
	public:
		Light(eLightType type = eLightType::kPoint) : m_type(int(type)), m_enabled(false),
			m_value(glm::vec3(1.0f)),
			m_direction(glm::vec3(0.0f, 0.0f, -1.0f)),
			m_position(glm::vec3(0.0f, 0.0f, -1.0f)),
			m_spotDirection(glm::vec3(0.0f, 0.0f, -1.0f)),
			m_innerAngle(glm::cos(glm::radians(12.5f))), m_outerAngle(glm::cos(glm::radians(20.0f)))
		{}

		//General light parameters
		int m_type;
		int m_enabled;
		float m_innerAngle, m_outerAngle;

		glm::vec3 m_value;			int pad1;
		glm::vec3 m_direction;		int pad2;
		glm::vec3 m_position;		int pad3;
		glm::vec3 m_spotDirection;	int pad4;

		// NOTE: The inner and outer angles are measured in terms of their cosine value.
	};

	struct LightBlock {
		uint32_t m_num_lights;
		int padding1[3];	// to align next vec3 on 16-byte boundary
		Light m_lights[MAX_LIGHTS];
		int padding2[12];

	};
}
