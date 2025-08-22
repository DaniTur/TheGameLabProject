#pragma once

#include "Layer.h"

class LayerStack
{
public:

	LayerStack() = default;
	~LayerStack();

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* overlay);

	void PopLayer(Layer* layer);
	void PopOverlay(Layer* overlay);

	std::vector<Layer*>::iterator begin();
	std::vector<Layer*>::iterator end();

	std::vector<Layer*>::reverse_iterator rbegin();
	std::vector<Layer*>::reverse_iterator rend();

private:

	std::vector<Layer*> m_Layers;
	unsigned int m_LayerIndex = 0; // not overlays
};

