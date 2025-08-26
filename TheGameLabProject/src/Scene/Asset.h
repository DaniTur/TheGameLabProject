#pragma once

#include <Graphics/Model.h>
#include <glm.hpp>

struct AssetData {
	std::string type;
	std::string filePath;
	glm::vec3 position{0.0f};
	glm::vec3 rotation{0.0f};
	float scale = 1.0f;
	bool colissions = false;
};

class Asset
{
public:
	Asset();

	explicit Asset(AssetData& data); 

	AssetData& GetData();

	void Draw(Shader& shader);

private:

	AssetData m_Data;
	std::unique_ptr<Model> m_Model;
};

