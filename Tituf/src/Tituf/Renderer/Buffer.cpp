#include "tfpch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Tituf
{
	BufferLayout::BufferLayout()
		: m_Elements({}), m_Stride(0)
	{
	}
	BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
		: m_Elements(elements), m_Stride(0)
	{
		CalculateOffsetsAndStride();
	}

	BufferLayout::BufferLayout(std::vector<BufferElement> elements)
		: m_Elements(elements), m_Stride(0)
	{
		CalculateOffsetsAndStride();
	}

	void BufferLayout::CalculateOffsetsAndStride()
	{
		uint32_t offset = 0;
		m_Stride = 0;
		for (auto& element : m_Elements)
		{
			element.Offset = offset;
			offset += element.Size;
		}
		m_Stride = offset;
	}

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
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
				return new OpenGLVertexBuffer(vertices, size);
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

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
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
				return new OpenGLIndexBuffer(indices, size);
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