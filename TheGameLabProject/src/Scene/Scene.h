#pragma once

#include "Asset.h"
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
	
	std::vector<Asset*> m_AssetContainer;

	// Rendering
	Shader m_Shader;

	Model m_HandGun;
	Model m_Bagpack;
	Model m_WoodenBox;
	Model m_Mp7;

	WorldTransform m_WorldTransform;
	WorldTransform m_handGunWorldTransform;
	WorldTransform m_woodenBoxesWorldTransform;
	WorldTransform m_mp7WorldTransform;

	ProjectionTransform m_projectionTransform;
};

