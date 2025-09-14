#include "pch.h"
#include "GameObject.h"

GameObject::GameObject(AssetManager& assetManager) 
	: m_SceneAssetManager(assetManager)
{
}

GameObject::GameObject(const GameObjectData& data, const Transform& transform, AssetManager& assetManager)
	: m_Data(data), m_Transform(transform), m_SceneAssetManager(assetManager)
{
	m_Model = m_SceneAssetManager.LoadModel(m_Data.filePath);
	LOG_TRACE("GameObject '{}' created", m_Data.filePath.c_str());
}

GameObjectData& GameObject::GetData()
{
	return m_Data;
}

Transform& GameObject::GetTransform()
{
	return m_Transform;
}

void GameObject::Draw(Shader& shader)
{
	m_Model->Draw(shader);
}