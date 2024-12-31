#pragma once

#include "Orange/Renderer/Texture.h"

namespace Orange
{

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return o_Width; }
		virtual uint32_t GetHeight() const override { return o_Height; }

		virtual void Bind(uint32_t slot = 0) const override;	

	private:
		std::string o_Path;
		uint32_t o_Width, o_Height;
		uint32_t o_RendererID;
	};

}