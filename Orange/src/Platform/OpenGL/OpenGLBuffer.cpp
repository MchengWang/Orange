#include "ogpch.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

namespace Orange
{

	//////////////////////////////////////////////////////////////////////
	////// VertexBuffer //////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		HZ_PROFILE_FUNCTION();

		glCreateBuffers(1, &o_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, o_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		HZ_PROFILE_FUNCTION();

		glDeleteBuffers(1, &o_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		HZ_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, o_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		HZ_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//////////////////////////////////////////////////////////////////////
	////// IndexBuffer ///////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		:o_Count(count)
	{
		HZ_PROFILE_FUNCTION();

		glCreateBuffers(1, &o_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, o_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		HZ_PROFILE_FUNCTION();

		glDeleteBuffers(1, &o_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		HZ_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, o_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		HZ_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}