#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	: m_vertices(vertices), 
	  m_indices(indices),
	  m_textures(textures)  {

	setupMesh();
}

void Mesh::Draw(const Shader& shader) {
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int ambientNr = 1;
	unsigned int normalNr = 1;
	unsigned int shininessNr = 1;
	for (unsigned int i = 0; i < m_textures.size(); i++) {
		// activate proper texture unit before binding
		glActiveTexture(GL_TEXTURE0 + i);
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = m_textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_ambient")
			number = std::to_string(ambientNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);
		else if (name == "texture_shininess")
			number = std::to_string(shininessNr++);

		std::string uniformName = name + number;
		glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
		shader.setInt(uniformName, i);
	}
	
	// draw mesh
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(m_indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh() {
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

	// VBO buffer layout
	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,Normal));

	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}
