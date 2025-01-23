#include "../include/Camera.h"

Camera::Camera() {
	m_cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	m_cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

	// The LookAt matrix transform uses the camera parameters as inputs(it also finds the missing attributes by calculating them with the inputs given),
	// and outputs a matrix to transform any point of the 3D world to the perspective of the camera
	m_view = glm::lookAt(m_cameraPosition,
					   m_cameraTarget,		
					   glm::vec3(0.0f, 1.0f, 0.0f));	// up vector in the world space
}

glm::mat4& Camera::getView() {

	m_view = glm::lookAt(m_cameraPosition,
						 m_cameraTarget,
						 glm::vec3(0.0f, 1.0f, 0.0f));

	return m_view;
}

void Camera::moveForward() {
	m_cameraPosition.z -= m_speed;
}

void Camera::moveBackward() {
	m_cameraPosition.z += m_speed;
}

void Camera::moveLeft() {
	m_cameraPosition.x -= 1.0f;
}

void Camera::moveRight() {
	m_cameraPosition.x += 1.0f;
}