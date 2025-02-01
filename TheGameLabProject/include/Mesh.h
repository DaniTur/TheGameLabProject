#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <shader.h>

using namespace std;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh {
public:
    // mesh data
    vector<Vertex>       m_vertices;
    vector<unsigned int> m_indices;
    vector<Texture>      m_textures;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

    void Draw(Shader& shader);

private:
    //  render data
    unsigned int m_VAO;
    unsigned int m_VBO;
    unsigned int m_EBO;

    void setupMesh();
};



