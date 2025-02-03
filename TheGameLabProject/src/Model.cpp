#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <stb_image.h>

#define ASSIMP_IMPORTER_POSTPROCESSING_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace)

Model::Model(const char* filepath) {
    loadModel(filepath);
}

void Model::Draw(Shader& shader) {
    for (auto &mesh : m_meshes) {
        mesh.Draw(shader);
    }
}

void Model::loadModel(const string &filepath) {

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath, ASSIMP_IMPORTER_POSTPROCESSING_FLAGS);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        return;
    }
    m_directory = filepath.substr(0, filepath.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(const aiNode* node, const aiScene* scene) {
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        const aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }
    // process all the child nodes (if any)
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(const aiMesh *mesh, const aiScene *scene) {

    vector<Vertex> vertices;
    vertices.reserve(mesh->mNumVertices);
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
        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        } else {
            vertex.Normal = glm::vec3(0.0f, 0.0f, 1.0f);
        }

        // Texture coordinates
        if (mesh->mTextureCoords[0]) {
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            glm::vec2 vector2;
            vector2.x = mesh->mTextureCoords[0][i].x;
            vector2.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vector2;
            //// tangent
            //vector.x = mesh->mTangents[i].x;
            //vector.y = mesh->mTangents[i].y;
            //vector.z = mesh->mTangents[i].z;
            //vertex.Tangent = vector;
            //// bitangent
            //vector.x = mesh->mBitangents[i].x;
            //vector.y = mesh->mBitangents[i].y;
            //vector.z = mesh->mBitangents[i].z;
            //vertex.Bitangent = vector;
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
       // TODO: cambiar por otro for recorriendo los indices de cada cara y añadirlos uno a uno
        //indices.insert(indices.end(), &face.mIndices[0], &face.mIndices[0] + face.mNumIndices);
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // Process textures/materials
    /*
        We assume a convention for sampler names in the shaders.Each diffuse texture should be named
        as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        Same applies to other texture as the following list summarizes:
        diffuse: texture_diffuseN
        specular: texture_specularN
        normal: texture_normalN
    */
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    // 1. diffuse maps
    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    
    return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(const aiMaterial* material, aiTextureType type, string typeName) {

    vector<Texture> meshTextures;
    
    for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {

        aiString file;
        material->GetTexture(type, i, &file);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skipTextureLoading = false;
        for (unsigned int j = 0; j < m_textures_loaded.size(); j++) {
            
            if (m_textures_loaded[j].path.compare(file.C_Str()) == 0) {

                meshTextures.push_back(m_textures_loaded[j]);
                skipTextureLoading = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skipTextureLoading) {
            Texture texture;
            texture.id = loadTextureFromFile(file.C_Str(), m_directory);
            texture.type = typeName;
            texture.path = file.C_Str();
            meshTextures.push_back(texture);
            m_textures_loaded.push_back(texture);
        }
    }
    
    return meshTextures;
}

unsigned int Model::loadTextureFromFile(const char *file, const string &directory) const {

    string fileName(file);
    fileName = directory + '/' + fileName;

    unsigned int textureID;
    glGenTextures(1, &textureID);
    if (textureID == 0) {
        std::cerr << "Error: OpenGL no generó un ID de textura válido." << std::endl;
    }

    int width;
    int height;
    int colorChannels;
    unsigned char *imageData = stbi_load(fileName.c_str(), &width, &height, &colorChannels, 0);

    if (!imageData) {

        std::cerr << "Texture failed to load at path: " << file << std::endl;
        stbi_image_free(imageData);

    } else {

        GLenum format = GL_RGB;
        if (colorChannels == 1)
            format = GL_RED;
        else if (colorChannels == 3)
            format = GL_RGB;
        else if (colorChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(imageData);
    }

    return textureID;
}
