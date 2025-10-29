#include "tfpch.h"
#include "Application.h"
#include "Event/ApplicationEvent.h"
#include "Log.h"

namespace Tituf
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		TF_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		// Create the window singleton
		//TFWindow& window = TFWindow::Get();
		std::cout << "Application created" << std::endl;
	}

	Application::~Application() = default;

	void Application::Run()
	{
		// Set event callbacks
		m_Window.SetAppEventCallback(std::bind(&Application::OnAppEvent, this, std::placeholders::_1));
		m_Window.SetKeyEventCallback(std::bind(&Application::OnKeyEvent, this, std::placeholders::_1));
		m_Window.SetMouseEventCallback(std::bind(&Application::OnMouseEvent, this, std::placeholders::_1));

		m_Window.Init();
		m_Window.InitGlewContext();

		while (running)
		{
			if (!m_Window.ProcessMessages())
				break;

			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_Window.SwapBuffers();
			Sleep(16); // simple frame limiter
		}
	}

	// --- Event handling ---
	void Application::OnAppEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) { return OnWindowResize(e); });
		dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) { return OnWindowClose(e); });
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		TF_CORE_TRACE("Handled resize: {0}x{1}", e.GetWidth(), e.GetHeight());
		return true;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		running = false;
		return true;
	}

	void Application::OnKeyEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& e) { return OnKeyPressEvent(e); });
		dispatcher.Dispatch<KeyRepeatEvent>([this](KeyRepeatEvent& e) { return OnKeyRepeatEvent(e); });
		dispatcher.Dispatch<KeyReleasedEvent>([this](KeyReleasedEvent& e) { return OnKeyReleasedEvent(e); });
		HandleLayersEvents(e);
	}

	bool Application::OnKeyPressEvent(KeyPressedEvent& e) { TF_CORE_TRACE("{0}", e.ToString()); return true; }
	bool Application::OnKeyRepeatEvent(KeyRepeatEvent& e) { TF_CORE_TRACE("{0}", e.ToString()); return true; }
	bool Application::OnKeyReleasedEvent(KeyReleasedEvent& e) { TF_CORE_TRACE("{0}", e.ToString()); return true; }

	void Application::OnMouseEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseMovedEvent>([this](MouseMovedEvent& e) { return OnMouseMovedEvent(e); });
		dispatcher.Dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent& e) { return OnMouseButtonPressedEvent(e); });
		dispatcher.Dispatch<MouseButtonReleasedEvent>([this](MouseButtonReleasedEvent& e) { return OnMouseButtonReleasedEvent(e); });
		HandleLayersEvents(e);
	}

	bool Application::OnMouseMovedEvent(MouseMovedEvent& e) { return true; }
	bool Application::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e) { TF_CORE_TRACE("{0}", e.ToString()); return true; }
	bool Application::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e) { TF_CORE_TRACE("{0}", e.ToString()); return true; }

	void Application::PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); layer->OnAttach(); }
	void Application::PushOverlay(Layer* overlay) { m_LayerStack.PushOverlay(overlay); overlay->OnAttach(); }

	void Application::HandleLayersEvents(Event& e)
	{
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.m_Handled)
				break;
		}
	}
}
