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

	void moveUp();

	void moveDown();

	void targetCenter();

	void setCameraSpeed(float deltaTime);

	void setCameraTarget(glm::vec3 direction);

private:

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::mat4 m_view;
	glm::vec3 m_up;
	float m_speed = 0.05f;
};

