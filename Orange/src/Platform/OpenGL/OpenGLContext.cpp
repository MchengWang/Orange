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
		OG_CORE_ASSERT(windowHandle, "window ���Ϊ�գ�");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(o_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		OG_CORE_ASSERT(status, "glad ��ʼ��ʧ�ܣ�");

		OG_CORE_INFO("OpenGL ����: ");
		OG_CORE_INFO("  ������: {0}", (char*)glGetString(GL_VENDOR));
		OG_CORE_INFO("  ��  ��: {0}", (char*)glGetString(GL_RENDERER));
		OG_CORE_INFO("  ��  ��: {0}", (char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(o_WindowHandle);
	}

}