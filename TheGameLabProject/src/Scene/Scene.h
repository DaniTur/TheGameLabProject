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

	void Render(Camera& camera);

	// Make Scene more abstract to use, enabling the use of the Scene as a container of elements
	std::vector<GameObject*>::const_iterator begin();
	std::vector<GameObject*>::const_iterator end();

private:

	std::string m_SceneID = "";
	std::string m_FilePath = "";
	
	std::vector<GameObject*> m_GameObjectContainer;

	// Rendering
	Shader m_Shader;

	ProjectionTransform m_projectionTransform;
};

