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

		virtual uint32_t GetColorAttachmentRendererID() const override { return o_ColorAttachment; }

		virtual const FramebufferSpecification& GetSpecification() const override { return o_Specification; }

	private:
		uint32_t o_RendererID;
		uint32_t o_ColorAttachment, o_DepathAttachment;
		FramebufferSpecification o_Specification;
	};

}