#pragma once
#include "tfpch.h"
#include <Tituf.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/glm.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"



class ExampleLayer : public Tituf::Layer
{
	public:
		ExampleLayer() : Layer("ExampleLayer"), m_Camera(glm::radians(45.0f), aspect, 0.1f, 100.0f) , m_SquarePosition(0.0f)
	{

		Tituf::MeshLoader loader;
		m_Meshes = loader.LoadModel("assets/models/fbx/Cube/RubixCube.fbx");

		//Assimp::Importer importer;
		//// Replace with path to a simple .fbx or .obj file
		//const aiScene* scene = importer.ReadFile("assets/models/fbx/Cube/RubixCube.fbx",
		//	aiProcess_Triangulate | aiProcess_FlipUVs);

		//if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		//	std::cerr << "Error loading model: " << importer.GetErrorString() << std::endl;
		//	return;
		//}
		//   
		//std::cout << "Model loaded successfully!\n";
		//std::cout << "Number of meshes: " << scene->mNumMeshes << std::endl;

		//for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		//	aiMesh* mesh = scene->mMeshes[i];
		//	std::cout << "Mesh " << i << " has " << mesh->mNumVertices << " vertices" << std::endl;
		//	m_Meshes.push_back(LoadMesh(mesh));
		//}

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

		m_Shader = Tituf::Ref<Tituf::Shader>(Tituf::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc));

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

		m_FlatColorShader = Tituf::Ref<Tituf::Shader>(Tituf::Shader::Create("FlatColor", flatShaderVertexSrc, flatShaderFragmentSrc));

		Tituf::Ref<Tituf::Shader> textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		Tituf::Ref<Tituf::Shader> CubeShader = m_ShaderLibrary.Load("assets/shaders/Cube.glsl");
		        
		m_Texture = Tituf::Ref<Tituf::Texture2D>(Tituf::Texture2D::Create("assets/textures/AnimeTest.png"));
		m_GokuLogo = Tituf::Ref<Tituf::Texture2D>(Tituf::Texture2D::Create("assets/textures/Goku.png"));


		std::dynamic_pointer_cast<Tituf::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Tituf::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);

	}
	virtual ~ExampleLayer() {}


	void CameraMovement(Tituf::Timestep ts)
	{
		// --- Camera movement ---
		glm::vec3 camMove(0.0f);

		if (Tituf::Input::IsKeyPressed(TF_KEY_W))
			camMove.z -= 1.0f; // forward
		if (Tituf::Input::IsKeyPressed(TF_KEY_S))
			camMove.z += 1.0f; // backward
		if (Tituf::Input::IsKeyPressed(TF_KEY_A))
			camMove.x -= 1.0f; // left
		if (Tituf::Input::IsKeyPressed(TF_KEY_D))
			camMove.x += 1.0f; // right
		if (Tituf::Input::IsKeyPressed(TF_KEY_DOWN))
			camMove.y -= 1.0f; // down
		if (Tituf::Input::IsKeyPressed(TF_KEY_UP))
			camMove.y += 1.0f; // up

		// Rotation
		if (Tituf::Input::IsKeyPressed(TF_KEY_LEFT))
			m_CameraYaw -= m_CameraRotationSpeed * ts.GetSecondes();
		if (Tituf::Input::IsKeyPressed(TF_KEY_RIGHT))
			m_CameraYaw += m_CameraRotationSpeed * ts.GetSecondes();

		// --- Compute forward and right vectors ---
		glm::vec3 forward;
		forward.x = sin(glm::radians(m_CameraYaw));
		forward.y = 0.0f;
		forward.z = -cos(glm::radians(m_CameraYaw));
		forward = glm::normalize(forward);

		glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

		// --- Apply movement ---
		m_CameraPosition += forward * camMove.z * m_CameraMovementSpeed * ts.GetSecondes();
		m_CameraPosition += right * camMove.x * m_CameraMovementSpeed * ts.GetSecondes();
		m_CameraPosition += glm::vec3(0.0f, 1.0f, 0.0f) * camMove.y * m_CameraMovementSpeed * ts.GetSecondes();

		// --- Update camera ---
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.LookAt(m_CameraPosition + forward);
	}
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
		Tituf::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Tituf::RenderCommand::Clear();

		CameraMovement(ts);

		Tituf::Renderer::BeginScene(m_Camera);  

		auto CubeShader = m_ShaderLibrary.Get("Cube");
		auto glCube = std::dynamic_pointer_cast<Tituf::OpenGLShader>(CubeShader);
		   
		glm::mat4 CubeTransform = glm::scale(glm::mat4(1.0f), glm::vec3(0.11f));
		glCube->Bind();
		glCube->UploadUniformFloat3("u_Color", m_SquareColor);
		   

		for (auto& meshVA : m_Meshes)
		{ 
			Tituf::Renderer::Submit(CubeShader, meshVA, CubeTransform);
		}

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Tituf::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Tituf::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++) 
			{

				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos + m_SquarePosition) * scale;
				//Tituf::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}	

		m_Texture->Bind();

		Tituf::Ref<Tituf::Shader> textureShader = m_ShaderLibrary.Get("Texture");
		//Tituf::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		 
		m_GokuLogo->Bind();
		//Tituf::Renderer::Submit(textureShader, m_SquareVA,
			//glm::translate(glm::mat4(1.0f),glm::vec3(0.5f,0.5f,0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));

		   
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
	Tituf::ShaderLibrary m_ShaderLibrary;
	Tituf::Ref<Tituf::Shader> m_Shader;
	Tituf::Ref<Tituf::VertexBuffer> m_VertexBuffer;
	Tituf::Ref<Tituf::IndexBuffer> m_IndexBuffer;
	Tituf::Ref<Tituf::VertexArray> m_VertexArray;

	Tituf::Ref<Tituf::Shader> m_FlatColorShader;
	Tituf::Ref<Tituf::Texture2D> m_Texture;
	Tituf::Ref<Tituf::Texture2D> m_GokuLogo;
	Tituf::Ref<Tituf::VertexArray> m_SquareVA; 
	Tituf::Ref<Tituf::VertexBuffer> m_SquareVB;
	Tituf::Ref<Tituf::IndexBuffer> m_SquareIB;
	std::vector<Tituf::Ref<Tituf::VertexArray>> m_Meshes;

	float aspect = 1180.0f / 680.0f; // your viewport size

	Tituf::PerspectiveCamera m_Camera;
	glm::vec3 m_CameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	float m_CameraMovementSpeed = 3.0f;
	float m_CameraRotationSpeed = 60.0f;
	float m_CameraRotation = 0.0f;
	float m_CameraYaw = 0.0f;    // rotation around Y axis
	float m_CameraPitch = 0.0f;  // optional: for looking up/down
  




	glm::vec3 m_Camera2DPosition;
	float m_Camera2DMovementSpeed = 3.0f;

	float m_Camera2DRotation = 0.0f;
	float m_Camera2DRotationSpeed = 90.0f;

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
	void CameraMovement();
};
 
Tituf::Application* Tituf::CreateApplication()
{
	return new SandBoxApp();
}	

