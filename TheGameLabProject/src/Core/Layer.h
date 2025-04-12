#pragma once

#include "Events/Event.h"

class Layer
{
public:

	explicit Layer(const std::string& name = "Layer");
	virtual ~Layer() = default;

	virtual void OnUpdate(double deltaTime) { /*Not mandatory for all layers*/ };
	virtual void OnRender() { /*Not mandatory for all layers*/ };
	virtual void OnEvent(Event& e) { /*Not mandatory for all layers*/ };

	virtual std::string GetName();

protected:
	std::string m_DebugName;
};

