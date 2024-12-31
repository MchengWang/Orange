#include "ogpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace Orange
{

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: o_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		OG_CORE_ASSERT(data, "º”‘ÿÕº∆¨ ß∞‹£°");

		o_Width = width;
		o_Height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &o_RendererID);
		glTextureStorage2D(o_RendererID, 1, GL_RGB8, o_Width, o_Height);

		glTextureParameteri(o_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(o_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(o_RendererID, 0, 0, 0, o_Width, o_Height, GL_RGB, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &o_RendererID);
	}
	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, o_RendererID);
	}

}