#pragma once

namespace Tituf
{
	enum class RendererAPI
	{
		None = 0, OpenGL = 1, Direct3D = 2, Metal = 3
	};

	class Renderer
	{
	public:
		static RendererAPI GetAPI();
	private:
		static RendererAPI s_RendererAPI;
	};
}
