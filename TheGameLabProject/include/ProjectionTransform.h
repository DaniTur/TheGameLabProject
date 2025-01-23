#pragma once

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

// Perspetive Projection
class ProjectionTransform
{
public:

	ProjectionTransform(int screenWidth, int screenHeight);

	void setFOV(float fov);

	void setMaxRenderDistance(float maxDistance);

	glm::mat4& getMatrix() const;

private:

	float m_aspectRatio;
	float m_FOV = 90.0f;	// vertical field of view
	float m_zNear = 0.1f;	// near plane
	float m_zFar = 100.0f;	// max render distance
};

