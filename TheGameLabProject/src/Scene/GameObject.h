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
	bool colissions = false;
};

struct Transform {
	glm::vec3 position{ 0.0f };
	glm::vec3 rotation{ 0.0f };
	glm::vec3 scale{ 1.0f };
	float uniformScale = 1.0f;
};

class GameObject
{
public:
	explicit GameObject(AssetManager& assetManager);

	GameObject(const GameObjectData& data, Transform& transform, AssetManager& assetManager);

	GameObjectData& GetData();
	Transform& GetTransform();

	void Draw(Shader& shader);

private:

	GameObjectData m_Data;
	std::shared_ptr<Model> m_Model;
	Transform m_Transform;

	AssetManager& m_SceneAssetManager;
};

