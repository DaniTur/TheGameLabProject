#include "../include/WorldTransform.h"

WorldTransform::WorldTransform() {
	m_translationVector = glm::vec3(0.0f, 0.0f, 0.0f);
	m_scaleVector = glm::vec3(1.0f, 1.0f, 1.0f);
	m_rotationAxis = glm::vec3(0.0f, 0.0f, 0.0f);
}

void WorldTransform::setTranslation(const glm::vec3& translation) {
	m_translationVector = translation;
}

void WorldTransform::setScale(float scale) {
	m_scaleFactor = scale;
}

void WorldTransform::setScale(const glm::vec3& scaleVector) {
	m_scaleVector = scaleVector;
}

void WorldTransform::setRotation(float angle, const glm::vec3& axis) {
	m_rotationAngle = angle;
	m_rotationAxis = axis;
}

glm::mat4& WorldTransform::getMatrix() const {

	glm::mat4 world(1.0f);

	// Scale
	world = glm::scale(world, m_scaleVector);
	world = glm::scale(world, glm::vec3(m_scaleFactor, m_scaleFactor, m_scaleFactor));	// uniform scale

	// Rotation
	world = glm::rotate(world, m_rotationAngle, m_rotationAxis);

	// Translation
	world = glm::translate(world, m_translationVector);

	return world;
}
