#include "ogpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Orange
{

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Int:      return GL_FLOAT;
		case ShaderDataType::Int2:     return GL_FLOAT;
		case ShaderDataType::Int3:     return GL_FLOAT;
		case ShaderDataType::Int4:     return GL_FLOAT;
		case ShaderDataType::Bool:     return GL_FLOAT;
		}

		OG_CORE_ASSERT(false, "未知的着色器数据类型！");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &o_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &o_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(o_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		OG_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "顶点缓冲区没有布局！");

		glBindVertexArray(o_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
			element.GetCpmponentCount(),
			ShaderDataTypeToOpenGLBaseType(element.Type),
			element.Normalized ? GL_TRUE : GL_FALSE,
			layout.GetStride(),
			(const void*)element.Offset);
			index++;
		}

		o_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(o_RendererID);
		indexBuffer->Bind();

		o_IndexBuffer = indexBuffer;
	}

}