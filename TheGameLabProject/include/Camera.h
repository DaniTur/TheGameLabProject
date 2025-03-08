#pragma once

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

class Camera
{
public:

	Camera();

	glm::mat4& getView();

	glm::vec3& getPosition();

	void moveForward(float deltaTime);

	void moveBackward(float deltaTime);

	void moveLeft(float deltaTime);

	void moveRight(float deltaTime);

	void moveUp(float deltaTime);

	void moveDown(float deltaTime);

	void targetCenter();

	void setCameraSpeed(float deltaTime);

	void setCameraTarget(glm::vec3 direction);

private:

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraTarget;
	glm::mat4 m_view;
	glm::vec3 m_up;
	float m_speed = 2.0f; // player movement speed
	float m_sensitivity = 1.0; // mouse sensitivity
};

