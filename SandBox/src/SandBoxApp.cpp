#pragma once
#include "tfpch.h"
#include <Tituf.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Platform/OpenGL/OpenGLShader.h"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"

class ExampleLayer : public Tituf::Layer
{
	public:
		ExampleLayer() : Layer("ExampleLayer"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_SquarePosition(0.0f)
	{
		m_VertexArray = Tituf::Ref<Tituf::VertexArray>(Tituf::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f ,0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f ,0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f ,0.2f, 1.0f
		};

		m_VertexBuffer = Tituf::Ref<Tituf::VertexBuffer>(Tituf::VertexBuffer::Create(vertices, sizeof(vertices)));
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

		m_IndexBuffer = Tituf::Ref<Tituf::IndexBuffer>(Tituf::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);


		float squareVertices[] = {
			// position            // color
			-0.5f, -0.5f, 0.0f,   1,0,0,1, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,   0,1,0,1, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,   0,0,1,1, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,   1,1,0,1, 0.0f, 1.0f
		};


		m_SquareVB = Tituf::Ref<Tituf::VertexBuffer>(Tituf::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		Tituf::BufferLayout squareVBLayout = {
			{Tituf::ShaderDataType::Float3, "a_Position"},
			{Tituf::ShaderDataType::Float4, "a_Color"},
			{Tituf::ShaderDataType::Float2, "a_TexCoord"}
		};

		m_SquareVB->SetLayout(squareVBLayout);
		m_SquareVA = Tituf::Ref<Tituf::VertexArray>(Tituf::VertexArray::Create());
		m_SquareVA->AddVertexBuffer(m_SquareVB);

		unsigned int squareIndices[6] = {
			0, 1, 2, 2, 3, 0
		};

		m_SquareIB = Tituf::Ref<Tituf::IndexBuffer>(Tituf::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(m_SquareIB);


		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
	
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;			
			out vec4 v_Color;			

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

		m_Shader = Tituf::Ref<Tituf::Shader>(Tituf::Shader::Create(vertexSrc, fragmentSrc));

		std::string flatShaderVertexSrc = R"(
		#version 330 core
    
		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec4 a_Color;

		uniform mat4 u_ViewProjection;
		uniform mat4 u_Transform;

		out vec3 v_Position;

		void main() {
			v_Position = a_Position;
			gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			//gl_Position = vec4(a_Position, 1.0);
		}
)";

		std::string flatShaderFragmentSrc = R"(
		#version 330 core

		layout(location = 0) out vec4 color;

		in vec3 v_Position;

		uniform vec3 u_Color;		

		void main()
		{
			float blue = 0.5 + v_Position.y * 0.5; // Blue changes with Y
			float green = 0.2 + v_Position.x * 0.3; // small green variation with X
			float red = 0.1;                        // small constant red
			//color = vec4(red, green, blue, 1.0);
			color = vec4(u_Color, 1.0);
		}
)";


		m_FlatColorShader = Tituf::Ref<Tituf::Shader>(Tituf::Shader::Create(flatShaderVertexSrc, flatShaderFragmentSrc));

		////////////////////////
		std::string textureShaderVertexSrc = R"(
		#version 330 core
    
		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec4 a_Color;
		layout(location = 2) in vec2 a_TexCoord;

		uniform mat4 u_ViewProjection;
		uniform mat4 u_Transform;

		out vec3 v_Position;
		out vec2 v_TexCoord;

		void main() {
			v_Position = a_Position;
 			v_TexCoord = a_TexCoord;
			gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			//gl_Position = vec4(a_Position, 1.0);
		}
)";

		std::string textureShaderFragmentSrc = R"(
		#version 330 core

		layout(location = 0) out vec4 color;

		in vec3 v_Position;
		in vec2 v_TexCoord;

		uniform sampler2D u_Texture;	     	

		void main()
		{
			color = texture(u_Texture, v_TexCoord);	
		}
)"; 


		m_TextureShader = Tituf::Ref<Tituf::Shader>(Tituf::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc));
		m_Texture = Tituf::Ref<Tituf::Texture2D>(Tituf::Texture2D::Create("assets/textures/AnimeTest.png"));

		std::dynamic_pointer_cast<Tituf::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Tituf::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);


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
		  

		if (Tituf::Input::IsKeyPressed(TF_KEY_J))
		{
			m_SquarePosition.x -= m_SquareMovementSpeed * ts;
		}
		else if (Tituf::Input::IsKeyPressed(TF_KEY_L))
		{
			m_SquarePosition.x += m_SquareMovementSpeed * ts;
		}

		if (Tituf::Input::IsKeyPressed(TF_KEY_I))
		{ 
			m_SquarePosition.y += m_SquareMovementSpeed * ts;
		}
		else if (Tituf::Input::IsKeyPressed(TF_KEY_K))
		{ 
			m_SquarePosition.y -= m_SquareMovementSpeed * ts;
		}

		Tituf::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Tituf::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Tituf::Renderer::BeginScene(m_Camera);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Tituf::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Tituf::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++) 
			{

				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos + m_SquarePosition) * scale;
				Tituf::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}	

		m_Texture->Bind();

		Tituf::Renderer::Submit(m_TextureShader	, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		//Tituf::Renderer::Submit(m_Shader, m_VertexArray);
		   
		Tituf::Renderer::EndScene();


	}
	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}
	virtual void OnEvent(Tituf::Event& event) override
	{
	}
private:
	Tituf::Ref<Tituf::Shader> m_Shader;
	Tituf::Ref<Tituf::VertexBuffer> m_VertexBuffer;
	Tituf::Ref<Tituf::IndexBuffer> m_IndexBuffer;
	Tituf::Ref<Tituf::VertexArray> m_VertexArray;

	Tituf::Ref<Tituf::Shader> m_FlatColorShader, m_TextureShader;
	Tituf::Ref<Tituf::Texture2D> m_Texture;
	Tituf::Ref<Tituf::VertexArray> m_SquareVA; 
	Tituf::Ref<Tituf::VertexBuffer> m_SquareVB;
	Tituf::Ref<Tituf::IndexBuffer> m_SquareIB;

	Tituf::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMovementSpeed = 3.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 90.0f;

	glm::vec3 m_SquarePosition;
	float m_SquareMovementSpeed = 1.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
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



