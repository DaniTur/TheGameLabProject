#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#define ASSIMP_IMPORTER_POSTPROCESSING_FLAGS (aiProcess_Triangulate | aiProcess_FlipUVs)

Model::Model(const char* filepath) {
    loadModel(filepath);
}

void Model::loadModel(string filepath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath, ASSIMP_IMPORTER_POSTPROCESSING_FLAGS);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        return;
    }
    m_directory = filepath.substr(0, filepath.find_first_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }
    // process all the child nodes (if any)
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {

    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    // Proccess vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {

        Vertex vertex;

        // Positions
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        // Normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

        // Texture coordinates
        glm::vec2 vector2;
        // TODO: vec.x = mesh->mTextureCoords[0][i].x; 
        vector2.x = mesh->mTextureCoords[0]->x;
        vector2.y = mesh->mTextureCoords[0]->y;
        vertex.TexCoords = vector2;

        vertices.push_back(vertex);
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
       // TODO: cambiar por otro for recorriendo los indices de cada cara y añadirlos uno a uno
        indices.insert(indices.end(), &face.mIndices[0], &face.mIndices[0] + face.mNumIndices);
    }

    // Process textures/materials
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        vector<Texture> diffuseTextures = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());
        
        vector<Texture> specularTextures = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularTextures.begin(), specularTextures.end());
    }
    
    return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, string typeName) {
    vector<Texture> textures;
    for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {

        aiString path;
        material->GetTexture(type, i, &path);
        Texture texture;
        texture.id = TextureFromFile(path.C_Str(), m_directory);
        texture.type = type;
        texture.path = path.C_Str();
        textures.push_back(texture);
    }
    
    return textures;
}
