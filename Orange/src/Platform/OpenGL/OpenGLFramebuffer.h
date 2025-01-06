#pragma once

#include "Orange/Renderer/FrameBuffer.h"

namespace Orange
{

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachmentRendererID() const override { return o_ColorAttachment; }

		virtual const FramebufferSpecification& GetSpecification() const override { return o_Specification; }

	private:
		uint32_t o_RendererID = 0;
		uint32_t o_ColorAttachment = 0, o_DepathAttachment = 0;
		FramebufferSpecification o_Specification;
	};

}