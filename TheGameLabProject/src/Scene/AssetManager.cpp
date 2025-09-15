#include "pch.h"
#include "AssetManager.h"

std::shared_ptr<Model> AssetManager::LoadModel(const std::filesystem::path &modelFilePath)
{
	// TODO: Check for modelFiles that are valid or not, and throw exception or just return and log.
	// if () { error } 

	// Checks if the Model already exists and return a shared pointer to that Model, if it doesnt exist or the map element has an empty pointer
	// then creates a new Model
	std::string fileName = modelFilePath.filename().string(); // filename with extension

	auto it = m_Models.find(fileName);
	if (it != m_Models.end()) {
		//LOG_INFO("[AssetManager] Model element '{}' in map already exists", fileName);
		if (!it->second.expired()) {
			LOG_DEBUG("[AssetManager] Model '{}' is already loaded, referencingad it from the cach map", fileName);
			return it->second.lock();
		}
		LOG_DEBUG("[AssetManager] Element in map already exists but the Model '{}' is not loaded", fileName);
	}

	auto modelPtr = std::make_shared<Model>(modelFilePath);
	m_Models[fileName] = modelPtr;

	LOG_INFO("[AssetManager] Loaded new Model '{}' and added to the cache map", fileName);
	return modelPtr;
}
