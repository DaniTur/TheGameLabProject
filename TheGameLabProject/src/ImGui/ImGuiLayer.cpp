#include "pch.h"
#include "ImGuiLayer.h"

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

// test
#include "KeyCodes.h"

ImGuiLayer::ImGuiLayer(Window& window)
	: Layer("ImGuiLayer"), m_Window(window)
{
	m_Active = false;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors; // Automatically change cursors(arrow, hand, resize arrows, etc)
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;  // Permits ImGui to move the mouse when the library needs

	//ImGui_ImplGlfw_InitForOpenGL(m_Window.get(), true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
}

ImGuiLayer::~ImGuiLayer()
{
	//ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();	
	ImGui::DestroyContext();
}

void ImGuiLayer::OnUpdate(double deltaTime)
{
	if (!m_Active) {
		return;
	}
	
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = static_cast<float>(deltaTime);
	io.DisplaySize = ImVec2((float)m_Window.GetWidth(), (float)m_Window.GetHeight());

	//ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	float transformX = 0.0f;
	float transformY = 0.0f;
	float transformZ = 0.0f;

	ImGui::ShowDemoWindow();
	//ImGui::Begin("Inspector");
	//ImGui::Text("Transform");
	//ImGui::Text("Position");
	//ImGui::SameLine();
	//ImGui::InputFloat("X", &transformX);
	//ImGui::SameLine();
	//ImGui::InputFloat("Y", &transformY);
	//ImGui::SameLine();
	//ImGui::InputFloat("Z", &transformZ);
	//ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//LOG_TRACE("ImGuiOverlay Update");
}

void ImGuiLayer::OnRender()
{
	if (!m_Active) {
		return;
	}
}

void ImGuiLayer::OnEvent(Event& event)
{
	if (!m_Active) {
		return;
	}

	switch (event.getEventType())
	{
		using enum EventType;	// reduce verbosity
	case KeyPressed:
		onKeyPressed(static_cast<KeyPressedEvent&>(event));
		break;
	case KeyReleased:
		onKeyReleased(static_cast<KeyReleasedEvent&>(event));
		break;
	case MouseMoved:
		onMouseMoved(static_cast<MouseMovedEvent&>(event));
		break;
	case MouseButtonPressed:
		onMouseButtonPressed(static_cast<MouseButtonPressedEvent&>(event));
		break;
	case MouseButtonReleased:
		onMouseButtonReleased(static_cast<MouseButtonReleasedEvent&>(event));
		break;
	case MouseScrolled:
		onMouseScrolled(static_cast<MouseScrolledEvent&>(event));
		break;
	default:
		break;
	}
}

void ImGuiLayer::onKeyPressed(KeyPressedEvent& event)
{
	if (event.getKeyCode() == Key::F1) {
		ToggleLayerEvent toggleEvent("ImGuiLayer");
		m_EventCallback(toggleEvent);
	}

	ImGuiIO& io = ImGui::GetIO();
	ImGuiKey key = KeyMapToImGuiKey(event.getKeyCode());
	io.AddKeyEvent(key, true);
	event.handled = true;
	LOG_TRACE("[ImGuiLayer] KeyPressed {}", event.getKeyCode());
}

void ImGuiLayer::onKeyReleased(KeyReleasedEvent& event) {
	ImGuiIO& io = ImGui::GetIO();
	ImGuiKey key = KeyMapToImGuiKey(event.getKeyCode());
	io.AddKeyEvent(key, false);
	event.handled = true;
	LOG_TRACE("[ImGuiLayer] KeyReleased {}", event.getKeyCode());
}

void ImGuiLayer::onMouseMoved(MouseMovedEvent& event) {
	ImGuiIO& io = ImGui::GetIO();
	io.AddMousePosEvent(event.getX(), event.getY());
	event.handled = true;
}

void ImGuiLayer::onMouseButtonPressed(MouseButtonPressedEvent& event) {
	ImGuiIO& io = ImGui::GetIO();
	io.AddMouseButtonEvent(event.getButtonCode(), true);
	event.handled = true;
	LOG_TRACE("[ImGuiLayer] MouseButtonPressed {}", event.getButtonCode());
}

void ImGuiLayer::onMouseButtonReleased(MouseButtonReleasedEvent& event) {
	ImGuiIO& io = ImGui::GetIO();
	io.AddMouseButtonEvent(event.getButtonCode(), false);
	event.handled = true;
	LOG_TRACE("[ImGuiLayer] MouseButtonReleased {}", event.getButtonCode());
}

void ImGuiLayer::onMouseScrolled(MouseScrolledEvent& event)
{
	ImGuiIO& io = ImGui::GetIO();
	io.AddMouseWheelEvent(event.GetXOffset(), event.GetYOffset());
	event.handled = true;
}

ImGuiKey ImGuiLayer::KeyMapToImGuiKey(int nativeKeyCode)
{
	switch (nativeKeyCode)
	{
	case Key::W: return ImGuiKey_W;
	case Key::A: return ImGuiKey_A;
	case Key::S: return ImGuiKey_S;
	case Key::D: return ImGuiKey_D;
	default: return ImGuiKey_None;
	}
}

void ImGuiLayer::SetEventCallback(const EventCallback& callback)
{
	m_EventCallback = callback;
}
