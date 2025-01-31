#pragma once

#include "Mesh.h"
#include <assimp/scene.h>

class Model
{
public:

	Model(const char* filepath);

private:

	vector<Mesh> m_meshes;
	string m_directory;

	// assimp
	void loadModel(string filepath);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial *material, aiTextureType type, string typeName);
};

