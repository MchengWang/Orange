#include "ogpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>

namespace Orange
{
	
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:o_WindowHandle(windowHandle)
	{
		OG_CORE_ASSERT(windowHandle, "window ¾ä±úÎª¿Õ£¡");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(o_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		OG_CORE_ASSERT(status, "glad ³õÊ¼»¯Ê§°Ü£¡");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(o_WindowHandle);
	}

}