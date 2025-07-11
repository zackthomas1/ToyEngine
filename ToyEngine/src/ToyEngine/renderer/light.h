#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ToyEngine
{
	struct DirectionalLight 
	{
		glm::vec3 direction,
		ambient, diffuse, specular;
	};
}
