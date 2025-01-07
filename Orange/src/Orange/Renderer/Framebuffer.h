#pragma once

#include "Orange/Core/Base.h"

namespace Orange
{

	struct FramebufferSpecification
	{
		uint32_t width = 0, height = 0;
		uint32_t Samples = 1; // 默认纹理数量

		bool SwapChainTarget = false; // 交换链开启标识
	};

	class Framebuffer
	{
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};

}
