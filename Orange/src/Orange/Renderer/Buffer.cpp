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
		case RendererAPI::None: OG_CORE_ASSERT(false, "��ǰ����֧����Ⱦ�ӿڣ�None!"); return nullptr;
		case RendererAPI::OpenGL: return new OpenGLVertexBuffer(vertices, size);
		}

		OG_CORE_ASSERT(false, "δ֪����Ⱦ�ӿڣ�");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None: OG_CORE_ASSERT(false, "��ǰ����֧����Ⱦ�ӿڣ�None!"); return nullptr;
		case RendererAPI::OpenGL: return new OpenGLIndexBuffer(indices, size);
		}

		OG_CORE_ASSERT(false, "δ֪����Ⱦ�ӿڣ�");
		return nullptr;
	}

}