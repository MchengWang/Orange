#pragma once

#include <string>

namespace Orange
{

	class Shader
	{
	public:
		Shader(const std::string& vertSrc, const std::string& fragSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

	private:
		uint32_t o_RendererID;
	};

}