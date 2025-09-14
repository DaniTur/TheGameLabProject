#pragma once
#include "Core/Layer.h"
#include "IEventEmitter.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include <Core/Window.h>
#include <Scene/Scene.h>
#include <imgui.h>
#include <filesystem>

class ImGuiLayer : public Layer, public IEventEmitter
{
public:

	explicit ImGuiLayer(Window& window, Scene& scene);
	~ImGuiLayer() override;

	void OnUpdate(double deltaTime) override;
	void OnRender() override;
	void OnEvent(Event& e) override;

	void SetEventCallback(const EventCallback& callback) override;

	void SetScene(Scene& scene);

private:
	void onKeyPressed(KeyPressedEvent& event);
	void onKeyReleased(KeyReleasedEvent& event);
	void onKeyTyped(KeyTypedEvent& event);
	void onMouseMoved(MouseMovedEvent& event);
	void onMouseButtonPressed(MouseButtonPressedEvent& event);
	void onMouseButtonReleased(MouseButtonReleasedEvent& event);
	void onMouseScrolled(MouseScrolledEvent& event);

	ImGuiKey KeyMapToImGuiKey(int nativeKeyCode);

private:

	EventCallback m_EventCallback;

	Window& m_Window;
	Scene& m_ActiveScene;
	std::filesystem::path m_currentFileBrowserPath;
	std::filesystem::path m_SelectedFilePath;

private:
	void DisplayFileList();
};

