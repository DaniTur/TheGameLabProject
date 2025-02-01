#pragma once

#include <vector>
#include "../include/CTexture.h"

class Cube {
public:

	Cube();

	~Cube();

	void draw() const;

	void bindTexture() const;

private:

	void load();

private:

	std::vector<float> m_vertices;

	unsigned int m_VBO = -1;
	unsigned int m_VAO = -1;

	CTexture m_texture;
};

