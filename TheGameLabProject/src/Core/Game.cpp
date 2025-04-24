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

	GameplayLayer* gameplayLayer = new GameplayLayer();
	gameplayLayer->SetEventCallback(std::bind_front(&Game::onEvent, this));
	m_LayerStack.PushLayer(gameplayLayer);

	//m_LayerStack.PushOverlay(new ImGuiLayer());
}

void Game::run() {

	m_running = true;
	LOG_INFO("Game running...");

	while (m_running) {

		// Timing
		FPSManager::Calculate();
		m_DeltaTime = FPSManager::GetDeltaTime();

		// Update
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
			(*it)->OnUpdate(m_DeltaTime);
		}

		m_window.onUpdate();
	}

	// Clearing all previously allocated GLFW resources.
	glfwTerminate();
}

// Don't generate Events in any of the OnEvent functions in any Layer
void Game::onEvent(Event& event)
{
	if (event.getEventType() == EventType::WindowClosed)
		onWindowClosed(static_cast<WindowClosedEvent&>(event));

	for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
		if (event.handled)
			break;
	
		(*it)->OnEvent(event);
	}
}

void Game::onWindowClosed(WindowClosedEvent& e)
{
	m_running = false;
	e.handled = true;
}