#include "pch.h"
#include "AssetManager.h"

std::shared_ptr<Model> AssetManager::LoadModel(const std::string& file)
{
	// Checks if the Model already exists and return a shared pointer to that Model, if it doesnt exist or the map element has an empty pointer
	// then creates a new Model
	auto it = m_Models.find(file);

	if (it != m_Models.end()) {
		LOG_INFO("[AssetManager] Model element '{}' in map already exists", file);
		if (!it->second.expired()) {
			LOG_INFO("[AssetManager] Model '{}' is already loaded", file);
			return it->second.lock();
		}
		LOG_INFO("[AssetManager] Element in map already exists but the Model '{}' is not loaded", file);
	}

	auto modelPtr = std::make_shared<Model>(file.c_str());
	m_Models[file] = modelPtr;

	LOG_INFO("[AssetManager] Loaded new Model '{}' and added to the map", file);
	return modelPtr;
}
