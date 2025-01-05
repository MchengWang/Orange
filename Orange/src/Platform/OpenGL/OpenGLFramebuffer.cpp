#include "ogpch.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Orange
{

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		:o_Specification(spec)
	{
		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &o_RendererID);
	}

	void OpenGLFramebuffer::Invalidate()
	{
		glCreateFramebuffers(1, &o_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, o_RendererID);

		glCreateTextures(GL_TEXTURE_2D, 1, &o_ColorAttachment);
		glBindTexture(GL_TEXTURE_2D, o_ColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, o_Specification.width, o_Specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, o_ColorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &o_DepathAttachment);
		glBindTexture(GL_TEXTURE_2D, o_DepathAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, o_Specification.width, o_Specification.height);

		//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, o_Specification.width, o_Specification.height, 0, 
		// GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, o_DepathAttachment, 0);

		OG_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, o_RendererID);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}