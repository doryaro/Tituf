#pragma once
#include "Tituf/Renderer/Buffer.h"
//#include "Tituf/Renderer/Renderer.h"
#include "memory.h"

namespace Tituf
{
	class VertexArray
	{
	public:  
		//VertexArray() {}
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;  

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;
		
		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		static VertexArray* Create();
	};
}
