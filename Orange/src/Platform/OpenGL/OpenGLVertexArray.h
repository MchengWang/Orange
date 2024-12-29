#pragma once

#include "Orange/Renderer/VertexArray.h"

namespace Orange
{

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return o_VertexBuffers; }
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return o_IndexBuffer; }

	private:
		uint32_t o_RendererID;
		std::vector<std::shared_ptr<VertexBuffer>> o_VertexBuffers;
		std::shared_ptr<IndexBuffer> o_IndexBuffer;

	};

}