#include "ogpch.h"
#include "Texture.h"

#include "Orange/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Orange
{ 
	
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: OG_CORE_ASSERT(false, "当前渲染接口：None 暂不支持！"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(path);
		}

		OG_CORE_ASSERT(false, "未知的渲染器接口！");
		return nullptr;
	}

}