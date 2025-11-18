#include "tfpch.h"
#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Tituf
{
	VertexArray* VertexArray::Create()
	{   
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
		{
			TF_CORE_ASSERT_INFO(false, "None RenderAPI not suppored!");
			return nullptr;
		}
		case RendererAPI::OpenGL:
		{
			return new OpenGLVertexArray();
		}
		case RendererAPI::Vulkan:
		{
			TF_CORE_ASSERT_INFO(false, "Vulkan RenderAPI not suppored!");
			return nullptr;
		}
		case RendererAPI::Direct3D:
		{
			TF_CORE_ASSERT_INFO(false, "Direct3D RenderAPI not suppored!");
			return nullptr;
		}
		case RendererAPI::Metal:
		{
			TF_CORE_ASSERT_INFO(false, "Metal RenderAPI not suppored!");
			return nullptr;
		}
		}
		TF_CORE_ASSERT_INFO(false, "unkown RenderAPI!");
		return nullptr;
	}
}