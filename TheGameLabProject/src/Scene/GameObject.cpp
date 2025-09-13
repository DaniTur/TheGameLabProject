#include "pch.h"
#include "GameObject.h"

GameObject::GameObject(AssetManager& assetManager) 
	: m_SceneAssetManager(assetManager)
{
}

GameObject::GameObject(const GameObjectData& data, Transform& transform, AssetManager& assetManager)
	: m_Data(data), m_Transform(transform), m_SceneAssetManager(assetManager)
{
	m_Model = m_SceneAssetManager.LoadModel(m_Data.filePath);
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