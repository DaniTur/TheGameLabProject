#include "../include/Camera.h"

Camera::Camera() {
	m_cameraPosition = glm::vec3(0.0f, 0.0f, 1.0f);
	// right handed system, target forward is along -Z axis
	m_cameraTarget = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4& Camera::getView() {
	// The LookAt matrix transform uses the camera parameters as inputs(it also finds the missing attributes by calculating them with the inputs given),
	// and outputs a matrix to transform any point of the 3D world to the perspective of the camera
	m_view = glm::lookAt(m_cameraPosition,
						 m_cameraTarget + m_cameraPosition,	// moves camera target along with the camera position
						 m_up);
	return m_view;
}

glm::vec3& Camera::getPosition()
{
	return m_cameraPosition;
}

void Camera::moveForward() {
	 m_cameraPosition += (m_cameraTarget * m_speed);
	//m_cameraPosition.z -= m_speed;
}

void Camera::moveBackward() {
	 m_cameraPosition -= (m_cameraTarget * m_speed);
	//m_cameraPosition.z += m_speed;
}

void Camera::moveLeft() {
	glm::vec3 left = glm::cross(m_cameraTarget, m_up);
	glm::normalize(left);
	left *= m_speed;
	// left is pointing at -X, result of orthogonal vector between up and target
	m_cameraPosition -= left;
}

void Camera::moveRight() {
	glm::vec3 right = glm::cross(m_cameraTarget,m_up);
	glm::normalize(right);
	right *= m_speed;
	m_cameraPosition += right;
}

void Camera::moveUp() {
	m_cameraPosition.y += m_speed;
}

void Camera::moveDown() {
	m_cameraPosition.y -= m_speed;
}

void Camera::targetCenter() {
	m_cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Camera::setCameraSpeed(float deltaTime) {
	m_speed *= deltaTime;
}

void Camera::setCameraTarget(glm::vec3 direction) {
	m_cameraTarget = direction;
}
