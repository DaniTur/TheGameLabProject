#pragma once

#include "Mesh.h"
#include <assimp/scene.h>

class Model
{
public:

	Model() = default;
	Model(const char* filepath);

	void Draw(Shader& shader);

private:

	std::vector<Mesh> m_meshes;
	// Assuming model file and textures are in the same directory
	std::string m_directory;
	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	std::vector<Texture> m_textures_loaded;
	// assimp
	void loadModel(const std::string &filepath);
	void processNode(const aiNode* node, const aiScene* scene);
	Mesh processMesh(const aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(const aiMaterial *material, aiTextureType type, std::string typeName);
	unsigned int loadTextureFromFile(const char* file, const std::string &directory) const;
	//debug
	void PrintMaterialInfo(const aiScene* scene);
	void VerifyEmbebedTextures(const aiScene* scene);
	void VerifyTexturesInMaterialProperties(const aiScene* scene);
};

