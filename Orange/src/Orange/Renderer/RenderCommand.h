#pragma once

#include "Orange/Renderer/RendererAPI.h"

namespace Orange
{

	class RenderCommand
	{
	public:
		inline static void Init()
		{
			o_RendererAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			o_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			o_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			o_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0)
		{
			o_RendererAPI->DrawIndexed(vertexArray, count);
		}

	private:
		static Scope<RendererAPI> o_RendererAPI;
	};

}