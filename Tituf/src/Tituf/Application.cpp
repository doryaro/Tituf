#include "tfpch.h"
#include "Application.h"
#include "Event/ApplicationEvent.h"
#include "Log.h"

namespace Tituf
{
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		// Create the window
		m_Window = std::make_unique<TFWindow>();

		// Set event callback after the window is created
		m_Window->SetAppEventCallback(std::bind(&Application::OnAppEvent , this, std::placeholders::_1));
		m_Window->SetKeyEventCallback(std::bind(&Application::OnKeyEvent, this, std::placeholders::_1));
		m_Window->SetMouseEventCallback(std::bind(&Application::OnMouseEvent, this, std::placeholders::_1));

		while (running)
		{
			// Process window messages
			if (!m_Window->ProcessMessages())
			{
				TF_CORE_INFO("Window closed");
				break;
			}
			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate(0.016f); // assuming a fixed timestep for simplicity	
			}
			
			Sleep(16); // ~60 FPS
			// TODO: Update, Render, etc.
		}
	}

	void Application::OnAppEvent(Event& e)
	{
		//TF_CORE_TRACE("Event received: {0}", e.ToString());

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) { return OnWindowResize(e); });
		dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) { return OnWindowClose(e); });
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		TF_CORE_TRACE("Handled resize: {0}x{1}", e.GetWidth(), e.GetHeight());
		// Here you can update viewport, renderer, etc.
		return true;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		TF_CORE_TRACE("Handled close event");
		running = false; // stops the Run() loop cleanly
		return true;
	}

	void Application::OnKeyEvent(Event& e)
	{
		//TF_CORE_TRACE("Event received: {0}", e.ToString());

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& e) { return OnKeyPressEvent(e); });
		dispatcher.Dispatch<KeyRepeatEvent>([this](KeyRepeatEvent& e) { return OnKeyRepeatEvent(e); });
		dispatcher.Dispatch<KeyReleasedEvent>([this](KeyReleasedEvent& e) { return OnKeyReleasedEvent(e); });
		HandleLayersEvents(e);

	}

	bool Application::OnKeyPressEvent(Event& e)
	{
		TF_CORE_TRACE("Event received: {0}", e.ToString());
		return true;
	}  
	bool Application::OnKeyRepeatEvent(Event& e)
	{
		TF_CORE_TRACE("Event received: {0}", e.ToString());
		return true;
	}
	bool Application::OnKeyReleasedEvent(Event& e)
	{
		TF_CORE_TRACE("Event received: {0}", e.ToString());
		return true;
	}

	void Application::OnMouseEvent(Event& e)
	{
		//TF_CORE_TRACE("Event received: {0}", e.ToString());

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseMovedEvent>([this](MouseMovedEvent& e) { return OnMouseMovedEvent(e); });
		dispatcher.Dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent& e) { return OnMouseButtonPressedEvent(e); });
		dispatcher.Dispatch<MouseButtonReleasedEvent>([this](MouseButtonReleasedEvent& e) { return OnMouseButtonReleasedEvent(e); });
		HandleLayersEvents(e);
	}
	bool Application::OnMouseMovedEvent(Event& e)
	{
		//TF_CORE_TRACE("Event received: {0}", e.ToString());
		return true;
	}
	bool Application::OnMouseButtonPressedEvent(Event& e)
	{
		TF_CORE_TRACE("Event received: {0}", e.ToString());
		return true;
	}
	bool Application::OnMouseButtonReleasedEvent(Event& e)
	{
		TF_CORE_TRACE("Event received: {0}", e.ToString());
		return true;
	}
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

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
