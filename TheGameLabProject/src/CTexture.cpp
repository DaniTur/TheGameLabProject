#include "CTexture.h"


CTexture::CTexture(unsigned int target, const std::string& filePath) : m_target(target) {

	// Generate texture object
	glGenTextures(1, &m_texture);
	// Bind the object to the target for the configuration settings
	glBindTexture(m_target, m_texture);

	// Texture default configuration
	// Set the texture wrapping parameters (on the currently bound texture object)
	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	loadImageData(filePath);

	// Once the texture is configured, unbind it from the target
	glBindTexture(m_target, 0);
}

void CTexture::bind(unsigned int textureUnit) const {
	glActiveTexture(textureUnit);
	glBindTexture(m_target, m_texture);
}

// PRIVATE
void CTexture::loadImageData(const std::string& filePath) const {
	// Load the image data to memory
	int textureWidth = 0;
	int textureHeight = 0;
	int numColorChannels = 0;
	unsigned char* textureImageData = stbi_load(filePath.c_str(), &textureWidth, &textureHeight, &numColorChannels, 0);
	// Check for data loading errors
	if (textureImageData) {
		// Generate the 2D texture from the image data loaded
		//			target, mipmap level, texture format, texture width, texture height, 0, image format, image datatype, image data
		glTexImage2D(m_target, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImageData);
		//glGenerateMipmap(GL_TEXTURE_2D);	// Generate texture copies of different resolutions
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(textureImageData);	// Free the image memory
}
