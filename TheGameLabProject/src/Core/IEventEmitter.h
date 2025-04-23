#pragma once
#include <functional>
#include "Events/Event.h"

class IEventEmitter {
public:
	using EventCallback = std::function<void(Event&)>;
	virtual ~IEventEmitter() = default;
	virtual void SetEventCallback(const EventCallback& callback) = 0;
};