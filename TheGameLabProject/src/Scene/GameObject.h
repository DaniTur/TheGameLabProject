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
	GameObject();

	explicit GameObject(GameObjectData& data);

	GameObjectData& GetData();

	void Draw(Shader& shader);

private:

	GameObjectData m_Data;
	std::unique_ptr<Model> m_Model;
};

