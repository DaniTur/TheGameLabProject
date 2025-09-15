#pragma once

#include "GameObject.h"
// Temporary
#include <Graphics/WorldTransform.h>
#include <Graphics/ProjectionTransform.h>
#include <Graphics/Camera.h>

class Scene
{
public:

	Scene();
	
	explicit Scene(const std::string_view &id);

	void Load();

	void AddGameObjectWithModel(const std::filesystem::path& modelPath);

	void Render(Camera& camera);

	// Make Scene more abstract to use, enabling the use of the Scene as a container of elements
	std::vector<GameObject*>::const_iterator begin();
	std::vector<GameObject*>::const_iterator end();

private:

	std::string m_SceneID = "";
	std::filesystem::path m_SceneFilePath;	// Relative to project working dir
	
	std::vector<GameObject*> m_GameObjectContainer;

	AssetManager m_SceneAssetManager;

	// Rendering
	Shader m_Shader;

	ProjectionTransform m_projectionTransform;
};

