#include "ogpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Orange
{

	VertexArray* VertexArray::Create()
	{ 
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None: OG_CORE_ASSERT(false, "��ǰ��Ⱦ�ӿڣ�None �ݲ�֧�֣�"); return nullptr;
		case RendererAPI::OpenGL: return new OpenGLVertexArray();
		}

		OG_CORE_ASSERT(false, "δ֪����Ⱦ���ӿڣ�");
		return nullptr;
	}

}