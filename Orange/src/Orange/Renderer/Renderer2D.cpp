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

		float TexIndex;
		float TilingFactor;
	};

	struct Renderer2D2ata
	{
		static const uint32_t MaxModels = 20000;
		static const uint32_t MaxVerts = MaxModels * 4;
		static const uint32_t MaxDots = MaxModels * 6;
		static const uint32_t MaxTexSolts = 32; // GPU 最大支持的纹理插槽


		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVert* QuadVertBufferBase = nullptr;
		QuadVert* QuadVertBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTexSolts> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0--bindng->Whith Texture

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics stats;
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
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float, "a_TexIndex"},
			{ShaderDataType::Float, "a_TilingFactor"}
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

		int32_t samplers[r2s_Data.MaxTexSolts];
		for (uint32_t i = 0; i < r2s_Data.MaxTexSolts; i++)
			samplers[i] = i;

		r2s_Data.TextureShader = Shader::Create("assets/shaders/Texture.gsc");
		r2s_Data.TextureShader->Bind();
		r2s_Data.TextureShader->SetIntArray("u_Textures", samplers, r2s_Data.MaxTexSolts);

		// set all texture slots to 0
		r2s_Data.TextureSlots[0] = r2s_Data.WhiteTexture;

		r2s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		r2s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		r2s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		r2s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
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

		r2s_Data.TextureSlotIndex = 1;
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
		// Bindg texures
		for (uint32_t i = 0; i < r2s_Data.TextureSlotIndex; i++)
			r2s_Data.TextureSlots[i]->Bind();

		RenderCommand::DrawIndexed(r2s_Data.QuadVertexArray, r2s_Data.QuadIndexCount);
		r2s_Data.stats.DrawCalls++;
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();

		r2s_Data.QuadIndexCount = 0;
		r2s_Data.QuadVertBufferPtr = r2s_Data.QuadVertBufferBase;

		r2s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f}, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		HZ_PROFILE_FUNCTION();

		if (r2s_Data.QuadIndexCount >= Renderer2D2ata::MaxVerts)
			FlushAndReset();

		const float textureIndex = 0.0f; // White Texture
		const float tilingFactor = 1.0f; // 平衡因子

		glm::mat transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		r2s_Data.QuadVertBufferPtr->Position = transform * r2s_Data.QuadVertexPositions[0];
		r2s_Data.QuadVertBufferPtr->Color = color;
		r2s_Data.QuadVertBufferPtr->TexCoord = { 0.0f, 0.0f };
		r2s_Data.QuadVertBufferPtr->TexIndex = textureIndex;
		r2s_Data.QuadVertBufferPtr->TilingFactor = tilingFactor;
		r2s_Data.QuadVertBufferPtr++;

		r2s_Data.QuadVertBufferPtr->Position = transform * r2s_Data.QuadVertexPositions[1];
		r2s_Data.QuadVertBufferPtr->Color = color;
		r2s_Data.QuadVertBufferPtr->TexCoord = { 1.0f, 0.0f };
		r2s_Data.QuadVertBufferPtr->TexIndex = textureIndex;
		r2s_Data.QuadVertBufferPtr->TilingFactor = tilingFactor;
		r2s_Data.QuadVertBufferPtr++;

		r2s_Data.QuadVertBufferPtr->Position = transform * r2s_Data.QuadVertexPositions[2];
		r2s_Data.QuadVertBufferPtr->Color = color;
		r2s_Data.QuadVertBufferPtr->TexCoord = { 1.0f, 1.0f };
		r2s_Data.QuadVertBufferPtr->TexIndex = textureIndex;
		r2s_Data.QuadVertBufferPtr->TilingFactor = tilingFactor;
		r2s_Data.QuadVertBufferPtr++;

		r2s_Data.QuadVertBufferPtr->Position = transform * r2s_Data.QuadVertexPositions[3];
		r2s_Data.QuadVertBufferPtr->Color = color;
		r2s_Data.QuadVertBufferPtr->TexCoord = { 0.0f, 1.0f };
		r2s_Data.QuadVertBufferPtr->TexIndex = textureIndex;
		r2s_Data.QuadVertBufferPtr->TilingFactor = tilingFactor;
		r2s_Data.QuadVertBufferPtr++;

		r2s_Data.QuadIndexCount += 6;

		r2s_Data.stats.QuadCount++;
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

		if (r2s_Data.QuadIndexCount >= Renderer2D2ata::MaxVerts)
			FlushAndReset();

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < r2s_Data.TextureSlotIndex; i++)
		{
			if (*r2s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)r2s_Data.TextureSlotIndex;
			r2s_Data.TextureSlots[r2s_Data.TextureSlotIndex] = texture;
			r2s_Data.TextureSlotIndex++;
		}

		glm::mat transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		r2s_Data.QuadVertBufferPtr->Position = transform * r2s_Data.QuadVertexPositions[0];
		r2s_Data.QuadVertBufferPtr->Color = color;
		r2s_Data.QuadVertBufferPtr->TexCoord = { 0.0f, 0.0f };
		r2s_Data.QuadVertBufferPtr->TexIndex = textureIndex;
		r2s_Data.QuadVertBufferPtr->TilingFactor = tilingFactor;
		r2s_Data.QuadVertBufferPtr++;

		r2s_Data.QuadVertBufferPtr->Position = transform * r2s_Data.QuadVertexPositions[1];
		r2s_Data.QuadVertBufferPtr->Color = color;
		r2s_Data.QuadVertBufferPtr->TexCoord = { 1.0f, 0.0f };
		r2s_Data.QuadVertBufferPtr->TexIndex = textureIndex;
		r2s_Data.QuadVertBufferPtr->TilingFactor = tilingFactor;
		r2s_Data.QuadVertBufferPtr++;

		r2s_Data.QuadVertBufferPtr->Position = transform * r2s_Data.QuadVertexPositions[2];
		r2s_Data.QuadVertBufferPtr->Color = color;
		r2s_Data.QuadVertBufferPtr->TexCoord = { 1.0f, 1.0f };
		r2s_Data.QuadVertBufferPtr->TexIndex = textureIndex;
		r2s_Data.QuadVertBufferPtr->TilingFactor = tilingFactor;
		r2s_Data.QuadVertBufferPtr++;

		r2s_Data.QuadVertBufferPtr->Position = transform * r2s_Data.QuadVertexPositions[3];
		r2s_Data.QuadVertBufferPtr->Color = color;
		r2s_Data.QuadVertBufferPtr->TexCoord = { 0.0f, 1.0f };
		r2s_Data.QuadVertBufferPtr->TexIndex = textureIndex;
		r2s_Data.QuadVertBufferPtr->TilingFactor = tilingFactor;
		r2s_Data.QuadVertBufferPtr++;

		r2s_Data.QuadIndexCount += 6;

		r2s_Data.stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		HZ_PROFILE_FUNCTION();

		if (r2s_Data.QuadIndexCount >= Renderer2D2ata::MaxVerts)
			FlushAndReset();

		const float textureIndex = 0.0f; // White Texture
		const float tilingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		r2s_Data.QuadVertBufferPtr->Position = transform * r2s_Data.QuadVertexPositions[0];
		r2s_Data.QuadVertBufferPtr->Color = color;
		r2s_Data.QuadVertBufferPtr->TexCoord = { 0.0f, 0.0f };
		r2s_Data.QuadVertBufferPtr->TexIndex = textureIndex;
		r2s_Data.QuadVertBufferPtr->TilingFactor = tilingFactor;
		r2s_Data.QuadVertBufferPtr++;

		r2s_Data.QuadVertBufferPtr->Position = transform * r2s_Data.QuadVertexPositions[1];
		r2s_Data.QuadVertBufferPtr->Color = color;
		r2s_Data.QuadVertBufferPtr->TexCoord = { 1.0f, 0.0f };
		r2s_Data.QuadVertBufferPtr->TexIndex = textureIndex;
		r2s_Data.QuadVertBufferPtr->TilingFactor = tilingFactor;
		r2s_Data.QuadVertBufferPtr++;

		r2s_Data.QuadVertBufferPtr->Position = transform * r2s_Data.QuadVertexPositions[2];
		r2s_Data.QuadVertBufferPtr->Color = color;
		r2s_Data.QuadVertBufferPtr->TexCoord = { 1.0f, 1.0f };
		r2s_Data.QuadVertBufferPtr->TexIndex = textureIndex;
		r2s_Data.QuadVertBufferPtr->TilingFactor = tilingFactor;
		r2s_Data.QuadVertBufferPtr++;

		r2s_Data.QuadVertBufferPtr->Position = transform * r2s_Data.QuadVertexPositions[3];
		r2s_Data.QuadVertBufferPtr->Color = color;
		r2s_Data.QuadVertBufferPtr->TexCoord = { 0.0f, 1.0f };
		r2s_Data.QuadVertBufferPtr->TexIndex = textureIndex;
		r2s_Data.QuadVertBufferPtr->TilingFactor = tilingFactor;
		r2s_Data.QuadVertBufferPtr++;

		r2s_Data.QuadIndexCount += 6;

		r2s_Data.stats.QuadCount++;
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

		if (r2s_Data.QuadIndexCount >= Renderer2D2ata::MaxVerts)
			FlushAndReset();

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < r2s_Data.TextureSlotIndex; i++)
		{
			if (*r2s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)r2s_Data.TextureSlotIndex;
			r2s_Data.TextureSlots[r2s_Data.TextureSlotIndex] = texture;
			r2s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		r2s_Data.QuadVertBufferPtr->Position = transform * r2s_Data.QuadVertexPositions[0];
		r2s_Data.QuadVertBufferPtr->Color = color;
		r2s_Data.QuadVertBufferPtr->TexCoord = { 0.0f, 0.0f };
		r2s_Data.QuadVertBufferPtr->TexIndex = textureIndex;
		r2s_Data.QuadVertBufferPtr->TilingFactor = tilingFactor;
		r2s_Data.QuadVertBufferPtr++;

		r2s_Data.QuadVertBufferPtr->Position = transform * r2s_Data.QuadVertexPositions[1];
		r2s_Data.QuadVertBufferPtr->Color = color;
		r2s_Data.QuadVertBufferPtr->TexCoord = { 1.0f, 0.0f };
		r2s_Data.QuadVertBufferPtr->TexIndex = textureIndex;
		r2s_Data.QuadVertBufferPtr->TilingFactor = tilingFactor;
		r2s_Data.QuadVertBufferPtr++;

		r2s_Data.QuadVertBufferPtr->Position = transform * r2s_Data.QuadVertexPositions[2];
		r2s_Data.QuadVertBufferPtr->Color = color;
		r2s_Data.QuadVertBufferPtr->TexCoord = { 1.0f, 1.0f };
		r2s_Data.QuadVertBufferPtr->TexIndex = textureIndex;
		r2s_Data.QuadVertBufferPtr->TilingFactor = tilingFactor;
		r2s_Data.QuadVertBufferPtr++;

		r2s_Data.QuadVertBufferPtr->Position = transform * r2s_Data.QuadVertexPositions[3];
		r2s_Data.QuadVertBufferPtr->Color = color;
		r2s_Data.QuadVertBufferPtr->TexCoord = { 0.0f, 1.0f };
		r2s_Data.QuadVertBufferPtr->TexIndex = textureIndex;
		r2s_Data.QuadVertBufferPtr->TilingFactor = tilingFactor;
		r2s_Data.QuadVertBufferPtr++;

		r2s_Data.QuadIndexCount += 6;

		r2s_Data.stats.QuadCount++;
	}

	void Renderer2D::ResetStats()
	{
		memset(&r2s_Data.stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return r2s_Data.stats;
	}
}