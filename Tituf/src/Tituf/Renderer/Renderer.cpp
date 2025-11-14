#include "tfpch.h"
#include "Renderer.h"


namespace Tituf
{
	RendererAPI Renderer::s_RendererAPI = RendererAPI::OpenGL;

	RendererAPI Renderer::GetAPI()
	{
		return s_RendererAPI;
	}
}