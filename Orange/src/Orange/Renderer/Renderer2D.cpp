#include "ogpch.h"
#include "Orange/Renderer/Renderer2D.h"

#include "Orange/Renderer/VertexArray.h"
#include "Orange/Renderer/Shader.h"
#include "Orange/Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Orange
{

	struct QuadVert
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
	};

	struct Renderer2D2ata
	{
		const uint32_t MaxModels = 10000;
		const uint32_t MaxVerts = MaxModels * 4;
		const uint32_t MaxDots = MaxModels * 6;


		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVert* QuadVertBufferBase = nullptr;
		QuadVert* QuadVertBufferPtr = nullptr;
	};

	static Renderer2D2ata r2s_Data;

	void Renderer2D::Init()
	{
		HZ_PROFILE_FUNCTION();

		r2s_Data.QuadVertexArray = VertexArray::Create();

		r2s_Data.QuadVertexBuffer = VertexBuffer::Create(r2s_Data.MaxDots * sizeof(QuadVert));
		r2s_Data.QuadVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float2, "a_TexCoord"}
			});
		r2s_Data.QuadVertexArray->AddVertexBuffer(r2s_Data.QuadVertexBuffer);

		r2s_Data.QuadVertBufferBase = new QuadVert[r2s_Data.MaxVerts];

		uint32_t* quadIndices = new uint32_t[r2s_Data.MaxDots];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < r2s_Data.MaxDots; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, r2s_Data.MaxDots);
		r2s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;


		r2s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whithTextureData = 0xffffffff;
		r2s_Data.WhiteTexture->SetData(&whithTextureData, sizeof(uint32_t));

		r2s_Data.TextureShader = Shader::Create("assets/shaders/Texture.gsc");
		r2s_Data.TextureShader->Bind();
		r2s_Data.TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		HZ_PROFILE_FUNCTION();

	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		HZ_PROFILE_FUNCTION();

		r2s_Data.TextureShader->Bind();
		r2s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjMatrix());

		r2s_Data.QuadIndexCount = 0;
		r2s_Data.QuadVertBufferPtr = r2s_Data.QuadVertBufferBase;
	}

	void Renderer2D::EndScene()
	{
		HZ_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)r2s_Data.QuadVertBufferPtr - (uint8_t*)r2s_Data.QuadVertBufferBase;
		r2s_Data.QuadVertexBuffer->SetData(r2s_Data.QuadVertBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		RenderCommand::DrawIndexed(r2s_Data.QuadVertexArray, r2s_Data.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f}, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		HZ_PROFILE_FUNCTION();

		r2s_Data.QuadVertBufferPtr->Position = position;
		r2s_Data.QuadVertBufferPtr->Color = color;
		r2s_Data.QuadVertBufferPtr->TexCoord = { 0.0f, 0.0f };
		r2s_Data.QuadVertBufferPtr++;

		r2s_Data.QuadVertBufferPtr->Position = { position.x + size.x, position.y, 0.0f};
		r2s_Data.QuadVertBufferPtr->Color = color;
		r2s_Data.QuadVertBufferPtr->TexCoord = { 1.0f, 0.0f };
		r2s_Data.QuadVertBufferPtr++;

		r2s_Data.QuadVertBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		r2s_Data.QuadVertBufferPtr->Color = color;
		r2s_Data.QuadVertBufferPtr->TexCoord = { 1.0f, 1.0f };
		r2s_Data.QuadVertBufferPtr++;

		r2s_Data.QuadVertBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
		r2s_Data.QuadVertBufferPtr->Color = color;
		r2s_Data.QuadVertBufferPtr->TexCoord = { 0.0f, 1.0f };
		r2s_Data.QuadVertBufferPtr++;

		r2s_Data.QuadIndexCount += 6;

		/*glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		r2s_Data.TextureShader->SetMat4("u_Transform", transform);

		r2s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(r2s_Data.QuadVertexArray);*/
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
		float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
		float tilingFactor, const glm::vec4& tintColor)
	{
		HZ_PROFILE_FUNCTION();

		r2s_Data.TextureShader->SetFloat4("u_Color", tintColor);
		r2s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) 
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		r2s_Data.TextureShader->SetMat4("u_Transform", transform);

		r2s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(r2s_Data.QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		HZ_PROFILE_FUNCTION();

		r2s_Data.TextureShader->SetFloat4("u_Color", color);
		r2s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
		r2s_Data.WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		r2s_Data.TextureShader->SetMat4("u_Transform", transform);

		r2s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(r2s_Data.QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture,
		float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture,
		float tilingFactor, const glm::vec4& tintColor)
	{
		HZ_PROFILE_FUNCTION();

		r2s_Data.TextureShader->SetFloat4("u_Color", tintColor);
		r2s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		r2s_Data.TextureShader->SetMat4("u_Transform", transform);

		r2s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(r2s_Data.QuadVertexArray);
	}
}