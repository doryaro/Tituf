#pragma once

namespace Tituf
{
	enum class RendererAPI
	{
		None = 0, OpenGL = 1, Direct3D = 2, Vulkan = 3, Metal = 4
	};

	class Renderer
	{
	public:
		static RendererAPI GetAPI();
	private:
		static RendererAPI s_RendererAPI;
	};
}
