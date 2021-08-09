#pragma once
#include <glm/glm.hpp>

class PointLightSource {
public:
	PointLightSource(glm::vec3 position, glm::vec3 angles, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
	glm::vec3 position;
	glm::vec3 angles;
	glm::vec3 direction = glm::vec3(0, 1.0f, 0.0f);
	glm::vec3 color;

	float constant;
	float linear;
	float quadratic;
};
