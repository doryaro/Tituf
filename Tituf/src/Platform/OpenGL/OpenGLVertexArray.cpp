#include "tfpch.h"
#include "OpenGLVertexArray.h"
namespace Tituf
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Tituf::ShaderDataType::Float:  return GL_FLOAT;
		case Tituf::ShaderDataType::Float2: return GL_FLOAT;
		case Tituf::ShaderDataType::Float3: return GL_FLOAT;
		case Tituf::ShaderDataType::Float4: return GL_FLOAT;
		case Tituf::ShaderDataType::Mat3:   return GL_FLOAT;
		case Tituf::ShaderDataType::Mat4:   return GL_FLOAT;
		case Tituf::ShaderDataType::Int:    return  GL_INT;
		case Tituf::ShaderDataType::Int2:   return GL_INT;
		case Tituf::ShaderDataType::Int3:   return GL_INT;
		case Tituf::ShaderDataType::Int4:   return GL_INT;
		case Tituf::ShaderDataType::Bool:   return GL_BOOL;
		}
		TF_CORE_ASSERT_INFO(false, "Unknow ShaderDataType!");
		return 0;
	}


	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}
	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);  
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		TF_CORE_ASSERT_INFO(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");
		
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const BufferLayout& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetElementCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)(uintptr_t)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}
		
	OpenGLVertexArray::~OpenGLVertexArray() 
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}
}