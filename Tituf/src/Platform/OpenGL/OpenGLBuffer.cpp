#include "tfpch.h"
#include "OpenGLBuffer.h"
namespace Tituf
{

/////////////////////////////////////////////////////////////////////////
/////////////// VertexBuffer       ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{ 
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	const BufferLayout& OpenGLVertexBuffer::GetLayout() const
	{
		return m_Layout;
	}

	void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout)
	{
		m_Layout = layout;
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() 
	{
		glDeleteBuffers(1, &m_RendererID);
	}

/////////////////////////////////////////////////////////////////////////
/////////////// IndexBuffer       ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_RendererID);  
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count* sizeof(uint32_t), indices, GL_STATIC_DRAW);

	}
	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}
	void OpenGLIndexBuffer::UnBind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}
	 

}