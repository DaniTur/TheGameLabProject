#include "pch.h"

#include "Game.h"
#include "FPSManager.h"
#include "Graphics/shader.h"
#include "Graphics/Model.h"
#include "Graphics/WorldTransform.h"
#include "Events/MouseEvent.h"
#include "Input.h"
#include "ImGui/ImGuiLayer.h"
#include "Gameplay/GameplayLayer.h"

Game::Game() 
	: m_window(m_screenWidth, m_screenHeight), m_projectionTransform(m_screenWidth, m_screenHeight) {

	m_window.setEventCallback(std::bind_front(&Game::onEvent, this));

	Input::Init(m_window.get());

	auto gameplayLayer = new GameplayLayer();
	gameplayLayer->SetEventCallback(std::bind_front(&Game::onEvent, this));
	m_LayerStack.PushLayer(gameplayLayer);

	auto imGuiLayer = new ImGuiLayer(m_window);
	imGuiLayer->SetEventCallback(std::bind_front(&Game::onEvent, this));
	m_LayerStack.PushOverlay(imGuiLayer);
}

void Game::run() {

	m_running = true;
	LOG_INFO("[Game] Game running...");

	while (m_running) {

		// Timing
		FPSManager::Calculate();
		m_DeltaTime = FPSManager::GetDeltaTime();

		// Update layers of the LayerStack each frame
		for (Layer* layer : m_LayerStack) {
			layer->OnUpdate(m_DeltaTime);
		}

		m_window.onUpdate();
	}

	// Clearing all previously allocated GLFW resources.
	glfwTerminate();
}

// Don't generate Events in any of the OnEvent functions in any Layer
void Game::onEvent(Event& event)
{
	// If the event is an application event, let the application handle it
	if (event.getEventCathegory() == EventCathegory::Application) {
		if (event.getEventType() == EventType::WindowClosed) {
			onWindowClosed(static_cast<WindowClosedEvent&>(event));
		}
		if (event.getEventType() == EventType::ToggleLayer) {
			onToggleLayer(static_cast<ToggleLayerEvent&>(event));
		}
	}

	for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
		if (event.handled) {
			break;
		}
		(*it)->OnEvent(event);
	}
}

void Game::onWindowClosed(WindowClosedEvent& e)
{
	m_running = false;
	e.handled = true;
}

// Toggles the layer active or inactive if exists
void Game::onToggleLayer(ToggleLayerEvent& event)
{
	const std::string& layerId = event.getLayerId();
	// search the layer to toggle by layerId
	auto it = m_LayerStack.rbegin();
	bool found = false;
	while (it != m_LayerStack.rend() && !found) {
		if ((*it)->GetName().compare(layerId) == 0) {
			found = true;
		}
		else {
			++it;
		}
	}
	if (found)
	{
		(*it)->ToggleActive();
	}

	// De-activate the mouse cursor capture when ImGuiLayer is Active
	if (event.getLayerId().compare("ImGuiLayer") == 0) {
		if ((*it)->IsActive()) {
			m_window.setMouseCursorCapture(false);
		} else {
			m_window.setMouseCursorCapture(true);
		}
	}

	event.handled = true;
}