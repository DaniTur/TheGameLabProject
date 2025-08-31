#include "pch.h"
#include "Layer.h"

Layer::Layer(const std::string& name) : m_DebugName(name)
{
}

bool Layer::IsActive()
{
	return m_Active;
}

std::string Layer::GetName()
{
	return m_DebugName;
}
