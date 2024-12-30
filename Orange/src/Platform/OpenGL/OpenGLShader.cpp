#include "ogpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Orange
{
	OpenGLShader::OpenGLShader(const std::string& vertSrc, const std::string& fragSrc)
	{
		// Create an empty vertex shader handle
		GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's.c_str is NULL character terminated.
		const GLchar* source = vertSrc.c_str();
		glShaderSource(vertShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore
			glDeleteShader(vertShader);

			OG_CORE_ERROR("{0}", infoLog.data());
			OG_CORE_ASSERT(false, "¶¥µã×ÅÉ«Æ÷±àÒëÊ§°Ü£¡");

			return;
		}

		// Create an empty fragment shader handle
		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's.c_str is NULL character terminated.
		source = fragSrc.c_str();
		glShaderSource(fragShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragShader);

		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore
			glDeleteShader(fragShader);

			OG_CORE_ERROR("{0}", infoLog.data());
			OG_CORE_ASSERT(false, "Æ¬¶Î×ÅÉ«Æ÷±àÒëÊ§°Ü£¡");

			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		o_RendererID = glCreateProgram();
		GLuint program = o_RendererID;

		// Attach our shaders to our program
		glAttachShader(program, vertShader);
		glAttachShader(program, fragShader);

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLink = 0;
		glGetProgramiv(program, GL_LINK_STATUS,	(int*)&isLink);
		if (isLink == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore
			glDeleteProgram(program);
			
			// Don't leak shaders either
			glDeleteShader(vertShader);
			glDeleteShader(fragShader);

			OG_CORE_ERROR("{0}", infoLog.data());
			OG_CORE_ASSERT(false, "×ÅÉ«Æ÷Á´½ÓÊ§°Ü£¡");
			return;
		}

		// Always detach shaders after a successful link
		glDetachShader(program, vertShader);
		glDetachShader(program, fragShader);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(o_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(o_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(o_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(o_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(o_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(o_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(o_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(o_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(o_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

}