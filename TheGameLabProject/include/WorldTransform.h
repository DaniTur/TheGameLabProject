#pragma once

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

class WorldTransform
{
public:

	WorldTransform();

	void setTranslation(const glm::vec3& translation); // move

	void setScale(float scale);	// uniform scale

	void setScale(const glm::vec3& scaleVector);

	void setRotation(float angle, const glm::vec3& axis);

	glm::mat4& getMatrix() const;

private:

	glm::vec3 m_translationVector;

	float m_scaleFactor = 1.0f;
	glm::vec3 m_scaleVector;

	float m_rotationAngle = 0.0f;
	glm::vec3 m_rotationAxis;
};

