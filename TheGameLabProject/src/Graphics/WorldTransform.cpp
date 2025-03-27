#include "WorldTransform.h"

WorldTransform::WorldTransform() 
	: m_translationVector(0.0f, 0.0f, 0.0f),
		m_scaleVector(1.0f, 1.0f, 1.0f),
		m_rotationAxis(0.0f, 0.0f, 0.0f),
		m_worldTransform(1.0f) {
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
	// Rotation is disables by default to prevent errors, if the rotation axis vector is not like (0,0,0) rotation is allowed to prevent errors
	if (axis.x > 0.0f || axis.y > 0.0f || axis.z > 0.0f) {
		m_rotate = true;
	}
}

glm::mat4 WorldTransform::getMatrix() const {

	glm::mat4 worldTransform(1.0f);

	worldTransform = glm::scale(worldTransform, m_scaleVector);
	worldTransform = glm::scale(worldTransform, glm::vec3(m_scaleFactor, m_scaleFactor, m_scaleFactor));

	if (m_rotate) {
		worldTransform = glm::rotate(worldTransform, m_rotationAngle, m_rotationAxis);
	}

	worldTransform = glm::translate(worldTransform, m_translationVector);

	return worldTransform;
}
