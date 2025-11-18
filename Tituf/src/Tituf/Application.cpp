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
		TF_CORE_ASSERT(!s_Instance); // Application already exists! 
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

		m_VertexArray = std::shared_ptr<VertexArray>(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f ,0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f ,0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f ,0.2f, 1.0f
		};

		m_VertexBuffer = std::shared_ptr<VertexBuffer>(VertexBuffer::Create(vertices, sizeof(vertices)));
		m_VertexBuffer->Bind();  

		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
		};
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		
		unsigned int indices[] = {
			0, 1, 2
		};   

		m_IndexBuffer = std::shared_ptr<IndexBuffer>(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);


		float squareVertices[] = {
			// position            // color
			-0.75f, -0.75f, 0.0f,   1,0,0,1,
			 0.75f, -0.75f, 0.0f,   0,1,0,1,
			 0.75f,  0.75f, 0.0f,   0,0,1,1,
			-0.75f,  0.75f, 0.0f,   1,1,0,1
		};


		m_SquareVB = std::shared_ptr<VertexBuffer>(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		BufferLayout squareVBLayout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"}
		};

		m_SquareVB->SetLayout(squareVBLayout);
		m_SquareVA = std::shared_ptr<VertexArray>(VertexArray::Create());
		m_SquareVA->AddVertexBuffer(m_SquareVB);

		unsigned int squareIndices[6] = {
			0, 1, 2, 2, 3, 0
		};

		m_SquareIB = std::shared_ptr<IndexBuffer>(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(m_SquareIB);
		 

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
	
			out vec3 v_Position;			
			out vec4 v_Color;			

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
			}  
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
	
			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				//color = vec4(v_Position * 0.5 + 0.5 , 1.0);
				color = v_Color;
			}  
		)";

		m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

		std::string blueShaderVertexSrc = R"(
		#version 330 core
    
		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec4 a_Color;

		out vec3 v_Position;

		void main() {
			v_Position = a_Position;
			gl_Position = vec4(a_Position, 1.0);
		}
)";

		std::string blueShaderFragmentSrc = R"(
		#version 330 core

		layout(location = 0) out vec4 color;

		in vec3 v_Position;

		void main()
		{
			float blue = 0.5 + v_Position.y * 0.5; // Blue changes with Y
			float green = 0.2 + v_Position.x * 0.3; // small green variation with X
			float red = 0.1;                        // small constant red
			color = vec4(red, green, blue, 1.0);
		}
)";


		m_BlueShader = std::make_unique<Shader>(blueShaderVertexSrc, blueShaderFragmentSrc);

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


			m_BlueShader->Bind();
			m_SquareVA->Bind();
			glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
			     

			m_Shader->Bind();
			m_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
			


			// Errors
			GLenum err = glGetError();
			if (err != GL_NO_ERROR)
				std::cout << "GL ERROR: " << err << std::endl;
			

			//Input check
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
