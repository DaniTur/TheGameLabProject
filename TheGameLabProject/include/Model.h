#pragma once

#include "Mesh.h"
#include <assimp/scene.h>

class Model
{
public:

	Model(const char* filepath);

	void Draw(Shader& shader);

private:

	vector<Mesh> m_meshes;
	string m_directory;
	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	vector<Texture> m_textures_loaded;
	// assimp
	void loadModel(const string &filepath);
	void processNode(const aiNode* node, const aiScene* scene);
	Mesh processMesh(const aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(const aiMaterial *material, aiTextureType type, string typeName);
	unsigned int loadTextureFromFile(const char* file, const string &directory) const;
};

