#pragma once

#include <Graphics/Model.h>
#include <glm.hpp>

enum class GameObjectType {
	Default,
	Entity,
	LightObject
};

struct GameObjectData {
	GameObjectType type = GameObjectType::Default;
	std::string filePath;
	glm::vec3 position{0.0f};
	glm::vec3 rotation{0.0f};
	float scale = 1.0f;
	bool colissions = false;
};

class GameObject
{
public:
	GameObject();

	explicit GameObject(GameObjectData& data);

	GameObjectData& GetData();

	void Draw(Shader& shader);

private:

	GameObjectData m_Data;
	std::unique_ptr<Model> m_Model;
};

