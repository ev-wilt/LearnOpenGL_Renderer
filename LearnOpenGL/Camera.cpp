#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float fov) :
	position(position),
	localFront(front),
	localUp(up),
	fov(fov)
{
	view = glm::lookAt(position, position + front, up);
}

void Camera::update()
{
	view = glm::lookAt(position, position + localFront, localUp);
}


void Camera::updateDirection(float yaw, float pitch)
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	localFront = glm::normalize(direction);
}

void Camera::setPosition(glm::vec3 position)
{
	this->position = position;
}

glm::vec3 Camera::getPosition() const
{
	return position;
}

glm::vec3 Camera::getLocalFront()
{
	return localFront;
}

glm::vec3 Camera::getLocalUp()
{
	return localUp;
}

glm::mat4 Camera::getView() const
{
	return view;
}

void Camera::updateFov(float offset)
{
	fov -= offset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}

float Camera::getFov() const
{
	return fov;
}
