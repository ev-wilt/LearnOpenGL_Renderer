#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include <vector>

class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float fov);

	void update();
	void updateDirection(float yaw, float pitch);

	void setPosition(glm::vec3 position);
	glm::vec3 getPosition() const;

	glm::vec3 getLocalFront();

	glm::vec3 getLocalUp();

	glm::mat4 getView() const;

	void updateFov(float offset);
	float getFov() const;

private:
	glm::vec3 position;
	glm::vec3 localFront;
	glm::vec3 localUp;
	glm::mat4 view;
	float fov;
};

#endif