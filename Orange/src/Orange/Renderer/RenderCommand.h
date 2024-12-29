#pragma once

#include "RendererAPI.h"

namespace Orange
{

	class RenderCommand
	{
	public:
		inline static void SetClearColor(const glm::vec4& color)
		{
			o_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			o_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			o_RendererAPI->DrawIndexed(vertexArray);
		}

	private:
		static RendererAPI* o_RendererAPI;
	};

}