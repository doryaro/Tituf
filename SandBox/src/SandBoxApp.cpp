#pragma once
#include "tfpch.h"
#include <Tituf.h>

class ExampleLayer : public Tituf::Layer
{
	public:
		ExampleLayer() : Layer("ExampleLayer"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		m_VertexArray = std::shared_ptr<Tituf::VertexArray>(Tituf::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f ,0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f ,0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f ,0.2f, 1.0f
		};

		m_VertexBuffer = std::shared_ptr<Tituf::VertexBuffer>(Tituf::VertexBuffer::Create(vertices, sizeof(vertices)));
		m_VertexBuffer->Bind();

		Tituf::BufferLayout layout = {
			{Tituf::ShaderDataType::Float3, "a_Position"},
			{Tituf::ShaderDataType::Float4, "a_Color"},
		};
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		unsigned int indices[] = {
			0, 1, 2
		};

		m_IndexBuffer = std::shared_ptr<Tituf::IndexBuffer>(Tituf::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);


		float squareVertices[] = {
			// position            // color
			-0.75f, -0.75f, 0.0f,   1,0,0,1,
			 0.75f, -0.75f, 0.0f,   0,1,0,1,
			 0.75f,  0.75f, 0.0f,   0,0,1,1,
			-0.75f,  0.75f, 0.0f,   1,1,0,1
		};


		m_SquareVB = std::shared_ptr<Tituf::VertexBuffer>(Tituf::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		Tituf::BufferLayout squareVBLayout = {
			{Tituf::ShaderDataType::Float3, "a_Position"},
			{Tituf::ShaderDataType::Float4, "a_Color"}
		};

		m_SquareVB->SetLayout(squareVBLayout);
		m_SquareVA = std::shared_ptr<Tituf::VertexArray>(Tituf::VertexArray::Create());
		m_SquareVA->AddVertexBuffer(m_SquareVB);

		unsigned int squareIndices[6] = {
			0, 1, 2, 2, 3, 0
		};

		m_SquareIB = std::shared_ptr<Tituf::IndexBuffer>(Tituf::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(m_SquareIB);


		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
	
			uniform mat4 u_ViewProjection;

			out vec3 v_Position;			
			out vec4 v_Color;			

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
				//gl_Position = vec4(a_Position, 1.0);
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

		m_Shader = std::make_unique<Tituf::Shader>(vertexSrc, fragmentSrc);

		std::string blueShaderVertexSrc = R"(
		#version 330 core
    
		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec4 a_Color;

		uniform mat4 u_ViewProjection;

		out vec3 v_Position;

		void main() {
			v_Position = a_Position;
			gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			//gl_Position = vec4(a_Position, 1.0);
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


		m_BlueShader = std::make_unique<Tituf::Shader>(blueShaderVertexSrc, blueShaderFragmentSrc);

	}
	virtual ~ExampleLayer() {}
	virtual void OnAttach() override
	{   
		TF_INFO("ExampleLayer attached");
	}     
	virtual void OnDetach() override
	{
		TF_INFO("ExampleLayer detached");
	}
	virtual void OnUpdate(Tituf::Timestep ts) override
	{
		//TF_TRACE("Delta time : {0} , secondes", ts.GetSecondes());
		if (Tituf::Input::IsKeyPressed(TF_KEY_LEFT))
		{
			m_CameraPosition.x -= m_CameraMovementSpeed * ts;
		}
		else if (Tituf::Input::IsKeyPressed(TF_KEY_RIGHT))
		{
			m_CameraPosition.x += m_CameraMovementSpeed * ts;
		}

		if (Tituf::Input::IsKeyPressed(TF_KEY_UP))
		{
			m_CameraPosition.y += m_CameraMovementSpeed * ts;
		}
		else if (Tituf::Input::IsKeyPressed(TF_KEY_DOWN))
		{
			m_CameraPosition.y -= m_CameraMovementSpeed * ts;
		}

		if (Tituf::Input::IsKeyPressed(TF_KEY_Q))
		{
			m_CameraRotation += m_CameraRotationSpeed * ts;
		}
		else if (Tituf::Input::IsKeyPressed(TF_KEY_E))
		{
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		}
		  
		Tituf::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Tituf::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Tituf::Renderer::BeginScene(m_Camera);

		Tituf::Renderer::Submit(m_BlueShader, m_SquareVA);
		Tituf::Renderer::Submit(m_Shader, m_VertexArray);

		Tituf::Renderer::EndScene();


	}
	virtual void OnImGuiRender() override
	{
	}
	virtual void OnEvent(Tituf::Event& event) override
	{
	}
private:
	std::shared_ptr<Tituf::Shader> m_Shader;
	std::shared_ptr<Tituf::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Tituf::IndexBuffer> m_IndexBuffer;
	std::shared_ptr<Tituf::VertexArray> m_VertexArray;
					
	std::shared_ptr<Tituf::Shader> m_BlueShader;
	std::shared_ptr<Tituf::VertexArray> m_SquareVA;
	std::shared_ptr<Tituf::VertexBuffer> m_SquareVB;
	std::shared_ptr<Tituf::IndexBuffer> m_SquareIB;

	Tituf::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMovementSpeed = 1.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 90.0f;

};                       





class SandBoxApp : public Tituf::Application
{
public:
	SandBoxApp() 
	{
		PushLayer(new ExampleLayer());
	};
	~SandBoxApp() {};   

};
 
Tituf::Application* Tituf::CreateApplication()
{
	return new SandBoxApp();
}	



