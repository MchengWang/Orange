#pragma once


#include "Orange/Renderer/Buffer.h"

namespace Orange
{

	class OpenGLVertexBuffer : public VertexBuffer
	{

	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const BufferLayout& GetLayout() const override { return o_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { o_Layout = layout; }

	private:
		uint32_t o_RendererID;
		BufferLayout o_Layout;

	};

	class OpenGLIndexBuffer : public IndexBuffer
	{

	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual	~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetCount() const { return o_Count; }

	private:
		uint32_t o_RendererID;
		uint32_t o_Count;

	};

}