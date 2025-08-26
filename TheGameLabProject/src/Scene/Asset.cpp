#include "pch.h"
#include "Asset.h"

Asset::Asset()
{
}

Asset::Asset(AssetData& data)
{
	m_Data = data;
	m_Model = std::make_unique<Model>(m_Data.filePath.c_str());
}

AssetData& Asset::GetData()
{
	return m_Data;
}

void Asset::Draw(Shader& shader) 
{
	m_Model->Draw(shader);
}