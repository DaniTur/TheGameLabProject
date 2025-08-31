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

	virtual bool IsActive();
	virtual std::string GetName();

	void ToggleActive() {
		m_Active = !m_Active;
	}

protected:
	std::string m_DebugName;
	bool m_Active = true;
};

