#pragma once
#include <Graphics/Model.h>

class AssetManager
{
public:

	std::shared_ptr<Model> LoadModel(const std::filesystem::path& modelFilePath);

private:

	std::unordered_map<std::string, std::weak_ptr<Model>> m_Models;
};

