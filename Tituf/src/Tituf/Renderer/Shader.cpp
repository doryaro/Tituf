#include "tfpch.h"
#include "Renderer.h"
#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Tituf
{
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
		{
			TF_CORE_ASSERT_INFO(false, "None RenderAPI not suppored!");
			return nullptr;
		}
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLShader>(filepath);
		}
		case RendererAPI::API::Vulkan:
		{
			TF_CORE_ASSERT_INFO(false, "Vulkan RenderAPI not suppored!");
			return nullptr;
		}
		case RendererAPI::API::Direct3D:
		{
			TF_CORE_ASSERT_INFO(false, "Direct3D RenderAPI not suppored!");
			return nullptr;
		}
		case RendererAPI::API::Metal:
		{
			TF_CORE_ASSERT_INFO(false, "Metal RenderAPI not suppored!");
			return nullptr;
		}
		}
		TF_CORE_ASSERT_INFO(false, "unkown RenderAPI!");
		return nullptr;
	}


	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
		{
			TF_CORE_ASSERT_INFO(false, "None RenderAPI not suppored!");
			return nullptr;
		}
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}
		case RendererAPI::API::Vulkan:
		{
			TF_CORE_ASSERT_INFO(false, "Vulkan RenderAPI not suppored!");
			return nullptr;
		}
		case RendererAPI::API::Direct3D:
		{
			TF_CORE_ASSERT_INFO(false, "Direct3D RenderAPI not suppored!");
			return nullptr;
		}
		case RendererAPI::API::Metal:
		{
			TF_CORE_ASSERT_INFO(false, "Metal RenderAPI not suppored!");
			return nullptr;
		}
		}
		TF_CORE_ASSERT_INFO(false, "unkown RenderAPI!");
		return nullptr;
	}


	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		TF_CORE_ASSERT_INFO(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	Tituf::Ref<Tituf::Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Tituf::Ref<Tituf::Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Tituf::Ref<Tituf::Shader> ShaderLibrary::Get(const std::string& name)
	{
		TF_CORE_ASSERT_INFO(Exists(name), "Shader not found!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}