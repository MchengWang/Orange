#pragma once

namespace Orange
{

	enum class RendererAPI
	{
		None = 0, OpenGL = 1
	};

	class Renderer
	{
	public:
		inline static RendererAPI GetAPI() { return o_RendererAPI; }

	private:
		static RendererAPI o_RendererAPI;
	};

}