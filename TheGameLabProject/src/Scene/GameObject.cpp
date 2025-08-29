#include "pch.h"
#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::GameObject(GameObjectData& data)
{
	m_Data = data;
	m_Model = std::make_unique<Model>(m_Data.filePath.c_str());
}

GameObjectData& GameObject::GetData()
{
	return m_Data;
}

void GameObject::Draw(Shader& shader)
{
	m_Model->Draw(shader);
}