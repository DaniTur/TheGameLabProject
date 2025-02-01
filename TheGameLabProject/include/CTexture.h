#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../include/stb_image.h"
#include <iostream>
#include <string>

class CTexture
{
public:
	
	CTexture() = default;

	CTexture(unsigned int target, const std::string& filePath);
	
	void bind(unsigned int textureUnit) const;

private:

	void loadImageData(const std::string& filePath) const;

	unsigned int m_texture = -1;
	unsigned int m_target = -1;
};

