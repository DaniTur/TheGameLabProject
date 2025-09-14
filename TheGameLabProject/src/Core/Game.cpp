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

	// Set the mouse start position at the center of the window
	m_Mouse.lastX = (float)m_window.GetWidth() / 2;
	m_Mouse.lastY = (float)m_window.GetHeight() / 2;

	Input::Init(m_window.get());

	auto gameplayLayer = new GameplayLayer();
	gameplayLayer->SetEventCallback(std::bind_front(&Game::onEvent, this));
	m_LayerStack.PushLayer(gameplayLayer);

	Scene& activeScene = gameplayLayer->GetActiveScene();

	auto imGuiLayer = new ImGuiLayer(m_window, activeScene);
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
		if (event.getEventType() == EventType::WindowResize) {
			onWindowResize(static_cast<WindowResizeEvent&>(event));
		}
	}

	if (event.getEventType() == EventType::MouseMoved) {
		onMouseMoved(static_cast<MouseMovedEvent&>(event));
	}

	for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
		if (event.handled) {
			break;
		}
		(*it)->OnEvent(event);
	}
}

void Game::onWindowClosed(WindowClosedEvent& event)
{
	m_running = false;
	event.handled = true;
}

void Game::onWindowResize(WindowResizeEvent& event) {
	// We are generating an event and setting the new size here, instead of setting the new size
	// directly in the callback within the Window class, this way we follow the same structure for 
	// all the events by not modifying any data in the glfw callbacks, do it in OnWhateverEvent() functions
	m_window.setNewSize(event.GetWidth(), event.GetHeight());
	glViewport(0, 0, event.GetWidth(), event.GetHeight());
	event.handled = true;
}

void Game::onMouseMoved(MouseMovedEvent& event) {
	// Calculate mouse offsets before the layers use the event
	if (m_Mouse.firstMouse) {
		m_Mouse.lastX = (float)m_window.GetWidth() / 2;
		m_Mouse.lastY = (float)m_window.GetHeight() / 2;
		event.setOffsetX(0.0f);
		event.setOffsetY(0.0f);
		m_Mouse.firstMouse = false;
		event.handled = true;
		return;
	}

	float xOffset = event.getX() - m_Mouse.lastX;
	float yOffset = m_Mouse.lastY - event.getY();
	m_Mouse.lastX = event.getX();
	m_Mouse.lastY = event.getY();
		
	event.setOffsetX(xOffset);
	event.setOffsetY(yOffset);
	// The proper layer will end up handling the event completly (the logical part of the mouse movement)
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
		} else {
			++it;
		}
	}
	if (found) {
		(*it)->ToggleActive();
	}
	// De-activate the mouse cursor capture when ImGuiLayer is Active
	if (event.getLayerId().compare("ImGuiLayer") == 0) {
		if ((*it)->IsActive()) {
			m_window.setMouseCursorCapture(false);
		} else {
			m_window.setMouseCursorCapture(true);
			resetMouseToCenterWindow();
		}
	}
	event.handled = true;
	LOG_TRACE("[Game] ToggleLayerEvent id: {}, to active: {}", event.getLayerId(), (*it)->IsActive());
}

void Game::resetMouseToCenterWindow() {
	float centerX = (float)m_window.GetWidth() / 2;
	float centerY = (float)m_window.GetHeight() / 2;
	m_window.setMousePosition(centerX, centerY);

	m_Mouse.lastX = centerX;
	m_Mouse.lastY = centerY;
	m_Mouse.firstMouse = true;
}

