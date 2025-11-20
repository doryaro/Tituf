#include "tfpch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Tituf
{
	VertexArray* VertexArray::Create()
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
			return new OpenGLVertexArray();
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
}