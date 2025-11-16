#pragma once
#include "Tituf/Renderer/Buffer.h"

namespace Tituf
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const;
		virtual void UnBind() const;

		virtual const BufferLayout& GetLayout() const override;
		virtual void SetLayout(const BufferLayout& layout) override;
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		virtual ~OpenGLIndexBuffer();
		OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
		virtual void Bind() const;
		virtual void UnBind() const; 

		virtual uint32_t GetCount() const { return m_Count;	};

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}
