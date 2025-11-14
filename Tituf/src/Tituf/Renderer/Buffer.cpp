#include "tfpch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Tituf
{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:
			{
				TF_CORE_ASSERT(false, "None RenderAPI not suppored!");
				return nullptr;
			}
			case RendererAPI::OpenGL:
			{
				return new OpenGLVertexBuffer(vertices, size);
			}
			case RendererAPI::Direct3D:
			{
				TF_CORE_ASSERT(false, "Direct3D RenderAPI not suppored!");
				return nullptr;
			}
			case RendererAPI::Metal:
			{
				TF_CORE_ASSERT(false, "Metal RenderAPI not suppored!");
				return nullptr;
			}
		}
		TF_CORE_ASSERT(false, "unkown RenderAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:
			{
				TF_CORE_ASSERT(false, "None RenderAPI not suppored!");
				return nullptr;
			}
			case RendererAPI::OpenGL: 
			{
				return new OpenGLIndexBuffer(indices, size);
			}
			case RendererAPI::Direct3D:
			{
				TF_CORE_ASSERT(false, "Direct3D RenderAPI not suppored!");
				return nullptr;
			}
			case RendererAPI::Metal:
			{
				TF_CORE_ASSERT(false, "Metal RenderAPI not suppored!");
				return nullptr;
			}
		}
		TF_CORE_ASSERT(false, "unkown RenderAPI!");
		return nullptr;
	}
}