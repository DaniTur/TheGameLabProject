#pragma once

#include "glm.hpp"
#include <string>
#include <vector>
#include "shader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    //glm::vec3 Tangent;
    //glm::vec3 Bitangent;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    // mesh data
    std::vector<Vertex>       m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture>      m_textures;
    unsigned int m_VAO;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    void Draw(const Shader& shader);

private:
    //  render data
    unsigned int m_VBO;
    unsigned int m_EBO;

    void setupMesh();
};



