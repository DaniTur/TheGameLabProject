#include "pch.h"
#include "LayerStack.h"

LayerStack::~LayerStack()
{
	for (Layer* layer : m_Layers) {
		delete layer;
	}
}

void LayerStack::PushLayer(Layer* layer)
{
	m_Layers.emplace(m_Layers.begin() + m_LayerIndex, layer);
	m_LayerIndex++;
}

void LayerStack::PushOverlay(Layer* overlay)
{
	m_Layers.emplace_back(overlay);
}

void LayerStack::PopLayer(Layer* layer)
{
	auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerIndex, layer);
	if (it != m_Layers.begin() + m_LayerIndex) {
		m_Layers.erase(it);
		m_LayerIndex--;
	}
}

void LayerStack::PopOverlay(Layer* overlay)
{
	auto it = std::find(m_Layers.begin() + m_LayerIndex, m_Layers.end(), overlay);
	if (it != m_Layers.end()) {
		m_Layers.erase(it);
	}
}

std::vector<Layer*>::reverse_iterator LayerStack::rbegin()
{
	return m_Layers.rbegin();
}

std::vector<Layer*>::reverse_iterator LayerStack::rend()
{
	return m_Layers.rend();
}
