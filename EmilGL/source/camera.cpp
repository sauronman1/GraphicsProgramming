#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

extern GLFWwindow* window;

glm::mat4 Camera::getViewMatrix() 
{
	glm::vec3 forward = rotation * glm::vec3(1.f, 0.f, 0.f);
	glm::vec3 right = rotation * glm::vec3(0.f, 0.f, 1.f);

	//Up Down
	if (glfwGetKey(window, GLFW_KEY_SPACE))
	{
		position.y += 0.05f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
	{
		position.y -= 0.05f;
	}

	//right left
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		position -= right * 0.05f;
	}
	if (glfwGetKey(window, GLFW_KEY_D))
	{
		position += right * 0.05f;
	}

	//forward back
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		position += forward * 0.05f;
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		position -= forward * 0.05f;
	}

	//Turn Right Left
	if (glfwGetKey(window, GLFW_KEY_E))
	{
		glm::quat turnDelta = glm::angleAxis(-0.01f, glm::vec3(0.f, 1.f, 0.f));
		rotation = turnDelta * rotation;
	}
	if (glfwGetKey(window, GLFW_KEY_Q))
	{
		glm::quat turnDelta = glm::angleAxis(0.01f, glm::vec3(0.f, 1.f, 0.f));
		rotation = turnDelta * rotation;

	}
	forward = rotation * glm::vec3(1.f, 0.f, 0.f);
	glm::vec3 center = position + forward;
		
	return glm::lookAt(position, center, glm::vec3(0.f, 1.f, 0.f));
}
