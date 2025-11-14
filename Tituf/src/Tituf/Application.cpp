#include "tfpch.h"
#include "Application.h"
#include "Event/ApplicationEvent.h"
#include "Log.h"
#include "ImGui/ImGuiLayer.h"
#include "Input.h"
namespace Tituf
{
	Application* Application::s_Instance = nullptr;
	
	Application::Application()
	{
		TF_CORE_ASSERT(!s_Instance) // Application already exists! 
		s_Instance = this;

		m_Window.Init();          // <--- init window
		m_Window.InitGlewContext(); // <--- init OpenGL
		m_Window.InitImguiContext(); // <--- init Imgui
		
		glViewport(0, 0, m_Window.GetData().Width, m_Window.GetData().Height);

		m_ImGuiLayer = new ImGuiLayer();	
		PushOverlay(m_ImGuiLayer);
		// Create the window singleton
		//TFWindow& window = TFWindow::Get();
		std::cout << "Application created" << std::endl;

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		m_VertexBuffer = std::unique_ptr<VertexBuffer>(VertexBuffer::Create(vertices, sizeof(vertices)));
		m_VertexBuffer->Bind();  

		 

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		 

		unsigned int indices[] = {
			0, 1, 2
		};   

		m_IndexBuffer = std::unique_ptr<IndexBuffer>(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));



		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
	
			void main()
			{
				gl_Position = vec4(a_Position, 1.0);
			}  
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
	
			void main()
			{
				color = vec4(0.8, 0.2, 0.3, 1.0);
			}  
		)";

		m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

	}

	Application::~Application() = default;

	void Application::Run()
	{
		// Set event callbacks
		m_Window.SetAppEventCallback(std::bind(&Application::OnAppEvent, this, std::placeholders::_1));
		m_Window.SetKeyEventCallback(std::bind(&Application::OnKeyEvent, this, std::placeholders::_1));
		m_Window.SetMouseEventCallback(std::bind(&Application::OnMouseEvent, this, std::placeholders::_1));
		     
		while (running)
		{
			if (!m_Window.ProcessMessages())
				break;


			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_Window.GlMakeCurrentImgui();
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();
			m_Window.SwapBuffersImgui();      // <--- swap buffers of the ImGui window


			m_Window.GlMakeCurrent();

			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_Shader->Bind();
			
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
			 
			GLenum err = glGetError();
			if (err != GL_NO_ERROR)
				std::cout << "GL ERROR: " << err << std::endl;



			bool APressed = Input::IsKeyPressed(TF_KEY_TAB); // Example usage of Input
			if (APressed)
			{
				TF_CORE_INFO("Key A is pressed");
			}	 

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
		glViewport(0, 0, e.GetWidth(), e.GetHeight());
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
		dispatcher.Dispatch<KeyTypedEvent>([this](KeyTypedEvent& e) { return OnKeyTypedEvent(e); });
		dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& e) { return OnKeyPressEvent(e); });
		dispatcher.Dispatch<KeyRepeatEvent>([this](KeyRepeatEvent& e) { return OnKeyRepeatEvent(e); });
		dispatcher.Dispatch<KeyReleasedEvent>([this](KeyReleasedEvent& e) { return OnKeyReleasedEvent(e); });
		HandleLayersEvents(e);
	}

	bool Application::OnKeyPressEvent(KeyPressedEvent& e) { TF_CORE_TRACE("{0}", e.ToString()); return true; }
	bool Application::OnKeyRepeatEvent(KeyRepeatEvent& e) { TF_CORE_TRACE("{0}", e.ToString()); return true; }
	bool Application::OnKeyReleasedEvent(KeyReleasedEvent& e) { TF_CORE_TRACE("{0}", e.ToString()); return true; }
	bool Application::OnKeyTypedEvent(KeyTypedEvent& e) { TF_CORE_TRACE("{0}", e.ToString()); return true; }


	void Application::OnMouseEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseMovedEvent>([this](MouseMovedEvent& e) { return OnMouseMovedEvent(e); });
		dispatcher.Dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent& e) { return OnMouseButtonPressedEvent(e); });
		dispatcher.Dispatch<MouseButtonReleasedEvent>([this](MouseButtonReleasedEvent& e) { return OnMouseButtonReleasedEvent(e); });
		HandleLayersEvents(e);
	}

	bool Application::OnMouseMovedEvent(MouseMovedEvent& e) { return true; }
 

	bool Application::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		// Get mouse position in screen coordinates
		POINT cursorPos;
		GetCursorPos(&cursorPos); // screen coordinates

		// Check which window the click was in
		HWND clickedWnd = WindowFromPoint(cursorPos);

		if (clickedWnd == m_Window.GetHwnd())
		{
			TF_CORE_INFO("Mouse clicked in MAIN window: button={0}, x={1}, y={2}",
				e.GetMouseButton(), cursorPos.x, cursorPos.y);
		}
		else if (clickedWnd == m_Window.GetImGuiHwnd())
		{
			TF_CORE_INFO("Mouse clicked in IMGUI window: button={0}, x={1}, y={2}",
				e.GetMouseButton(), cursorPos.x, cursorPos.y);
		}
		else
		{
			TF_CORE_INFO("Mouse clicked in OTHER window: button={0}, x={1}, y={2}",
				e.GetMouseButton(), cursorPos.x, cursorPos.y);
		}

		return true;
	}
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
