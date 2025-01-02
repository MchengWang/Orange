#include "ogpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Orange
{

	Scope<RendererAPI> RenderCommand::o_RendererAPI = CreateScope<OpenGLRendererAPI>();

}