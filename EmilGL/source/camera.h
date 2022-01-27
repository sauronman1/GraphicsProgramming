#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct Camera
{
	glm::vec3 position = glm::vec3(0.f);
	glm::quat rotation = glm::quat(1.f, 0.f, 0.f, 0.f);

	glm::mat4 getViewMatrix();
};