#pragma once

#include "Orange/Core/Base.h"

namespace Orange
{

	struct FramebufferSpecification
	{
		uint32_t width, height;
		uint32_t Samples = 1; // Ĭ����������

		bool SwapChainTarget = false; // ������������ʶ
	};

	class Framebuffer
	{
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};

}