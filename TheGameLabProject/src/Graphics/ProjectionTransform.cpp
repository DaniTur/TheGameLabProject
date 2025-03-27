#include "pch.h"

#include "ProjectionTransform.h"

ProjectionTransform::ProjectionTransform(int screenWidth, int screenHeight) {
	m_aspectRatio = (float)screenWidth / (float)screenHeight;
}

void ProjectionTransform::setFOV(float fov) {
	m_FOV = fov;
}

void ProjectionTransform::setMaxRenderDistance(float maxDistance) {
	m_zFar = maxDistance;
}

glm::mat4 ProjectionTransform::getMatrix() const {

	glm::mat4 projection(1.0f);

	projection = glm::perspective(m_FOV, m_aspectRatio, m_zNear, m_zFar);

	return projection;
}
