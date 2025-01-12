#pragma once

#include "Orange/Renderer/Texture.h"

#include <glad/glad.h>

namespace Orange
{

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return o_Width; }
		virtual uint32_t GetHeight() const override { return o_Height; }
		virtual uint32_t GetRendererID() const override { return o_RendererID; }
		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;	

		virtual bool IsLoaded() const override { return o_IsLoaded; }

		virtual bool operator==(const Texture& other) const override
		{
			return o_RendererID == ((OpenGLTexture2D&)other).o_RendererID;
		}

	private:
		std::string o_Path;
		bool o_IsLoaded = false;
		uint32_t o_Width, o_Height;
		uint32_t o_RendererID;
		GLenum o_InternalFormat, o_DataFormat;
	};

}
