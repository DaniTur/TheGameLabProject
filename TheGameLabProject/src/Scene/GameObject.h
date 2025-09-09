#pragma once

#include <glm.hpp>
#include <Graphics/Model.h>
#include <Scene/AssetManager.h>

enum class GameObjectType {
	Default,
	Entity,
	LightObject
};

struct GameObjectData {
	GameObjectType type = GameObjectType::Default;
	std::string filePath;
	std::string name;
	glm::vec3 position{0.0f};
	glm::vec3 rotation{0.0f};
	glm::vec3 scale{1.0f};
	float uniformScale = 1.0f;
	bool colissions = false;
};

class GameObject
{
public:
	explicit GameObject(AssetManager& assetManager);

	GameObject(const GameObjectData& data, AssetManager& assetManager);

	GameObjectData& GetData();

	void Draw(Shader& shader);

private:

	GameObjectData m_Data;
	std::shared_ptr<Model> m_Model;

	AssetManager& m_SceneAssetManager;
};

