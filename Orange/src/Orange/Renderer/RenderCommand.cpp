#include "ogpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Orange
{

	RendererAPI* RenderCommand::o_RendererAPI = new OpenGLRendererAPI;

}