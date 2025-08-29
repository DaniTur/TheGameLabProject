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

private:

	std::string m_SceneID = "";
	std::string m_FilePath = "";
	
	std::vector<GameObject*> m_GameObjectContainer;

	// Rendering
	Shader m_Shader;

	ProjectionTransform m_projectionTransform;
};

