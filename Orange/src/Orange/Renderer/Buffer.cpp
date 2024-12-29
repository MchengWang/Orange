#include "ogpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Orange
{

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None: OG_CORE_ASSERT(false, "当前并不支持渲染接口：None!"); return nullptr;
		case RendererAPI::OpenGL: return new OpenGLVertexBuffer(vertices, size);
		}

		OG_CORE_ASSERT(false, "未知的渲染接口！");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None: OG_CORE_ASSERT(false, "当前并不支持渲染接口：None!"); return nullptr;
		case RendererAPI::OpenGL: return new OpenGLIndexBuffer(indices, size);
		}

		OG_CORE_ASSERT(false, "未知的渲染接口！");
		return nullptr;
	}

}