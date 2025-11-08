#pragma once
#include "Core.h"
#include "Tituf/LayerStack.h"
#include "Tituf/ImGui/ImGuiLayer.h"	
#include "Event/Event.h"
#include "Window/TFWindow.h"

namespace Tituf
{
	class TITUF_API Application
	{
	public:
		// Singleton access
		static Application& Get() { return *s_Instance; }

		Application();
		virtual ~Application();

		void Run();

		// Event handling
		void OnAppEvent(Event& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowClose(WindowCloseEvent& e);

		void OnKeyEvent(Event& e);
		bool OnKeyPressEvent(KeyPressedEvent& e);
		bool OnKeyRepeatEvent(KeyRepeatEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);

		void OnMouseEvent(Event& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void HandleLayersEvents(Event& e);

	private:
		TFWindow& m_Window = TFWindow::Get(); // always points to the singleton
		bool running = true;
		LayerStack m_LayerStack; 
		ImGuiLayer* m_ImGuiLayer;
		static Application* s_Instance;

		unsigned int m_VertexArray;
		unsigned int m_VertexBuffer;
		unsigned int m_IndexBuffer;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}
