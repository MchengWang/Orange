#include "ogpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Orange
{

	Shader* Shader::Create(const std::string& vertSrc, const std::string& fragSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: OG_CORE_ASSERT(false, "当前暂不支持渲染接口：None！"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLShader(vertSrc, fragSrc);
		}

		OG_CORE_ASSERT(false, "未知的渲染接口！");
		return nullptr;
	}
}