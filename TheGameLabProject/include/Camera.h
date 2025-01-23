#pragma once

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

class Camera
{
public:

	Camera();

	glm::mat4& getView();

	void moveForward();

	void moveBackward();

	void moveLeft();

	void moveRight();

private:

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::mat4 m_view;
	float m_speed = 0.01f;
};

