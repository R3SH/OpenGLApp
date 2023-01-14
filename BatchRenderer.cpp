#include "BatchRenderer.h"

#include <iostream>
#include <array>
//#include <GL/glew.h>

//TODO: Add GPU based MaxTexture definition

static const size_t MaxQuadCount = 1000;		//Max n of quads to render
static const size_t MaxVertexCount = MaxQuadCount * 4;		//Max n of vertex to render
static const size_t MaxIndexCount = MaxQuadCount * 6;		//Max n of indexs to render
static const size_t MaxTextures = 32;		//Max n of textures defined by GPU

struct Vertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoords;
	float TexIndex;
};

struct BatchRendererData
{
	GLuint QuadVA = 0;
	GLuint QuadVB = 0;
	GLuint QuadIB = 0;

	GLuint WhiteTexture = 0;
	uint32_t WhiteTextureSlot = 0;

	uint32_t IndexCount = 0;

	Vertex* QuadBuffer = nullptr;
	Vertex* QuadBufferPtr = nullptr;

	std::array<uint32_t, MaxTextures> TextureSlots;
	uint32_t TextureSlotIndex = 1;

	BatchRenderer::Stats BatchRendererStats;
};

static BatchRendererData s_Data;

void BatchRenderer::Init()
{
	if (s_Data.QuadBuffer != nullptr)
	{
		std::cout << "BATCH RENDERER INIT FAIL: QuadBuffer isn't empty";
		Flush();
		system("PAUSE");
		//return;
	}

	s_Data.QuadBuffer = new Vertex[MaxVertexCount];

	GLCall(glCreateVertexArrays(1, &s_Data.QuadVA));
	GLCall(glBindVertexArray(s_Data.QuadVA));

	GLCall(glCreateBuffers(1, &s_Data.QuadVB));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, s_Data.QuadVB));
	GLCall(glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW));

	//Setting up offsets
	// position attribute
	GLCall(glEnableVertexArrayAttrib(s_Data.QuadVA, 0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position)));

	// color attribute
	GLCall(glEnableVertexArrayAttrib(s_Data.QuadVA, 1));
	GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color)));

	// texture attribute
	GLCall(glEnableVertexArrayAttrib(s_Data.QuadVA, 2));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords)));

	// texture index
	GLCall(glEnableVertexArrayAttrib(s_Data.QuadVA, 3));
	GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexIndex)));

	uint32_t indices[MaxIndexCount];
	uint32_t offset = 0;

	for (size_t i = 0; i < MaxIndexCount; i += 6)
	{
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;
		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;

		offset += 4;
	}

	GLCall(glCreateBuffers(1, &s_Data.QuadIB));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.QuadIB));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	//1x1 white texture that acts as no texture that won't affect the color to render
	GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &s_Data.WhiteTexture));
	GLCall(glBindTexture(GL_TEXTURE_2D, s_Data.WhiteTexture));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	uint32_t color = 0xffffffff;
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color));

	s_Data.TextureSlots[0] = s_Data.WhiteTexture;
	//use memset?
	for (size_t i = 1; i < MaxTextures; ++i)
		s_Data.TextureSlots[i] = 0;
}

void BatchRenderer::Shutdown()
{
	GLCall(glDeleteVertexArrays(1, &s_Data.QuadVA));
	GLCall(glDeleteBuffers(1, &s_Data.QuadVB));
	GLCall(glDeleteBuffers(1, &s_Data.QuadIB));

	GLCall(glDeleteTextures(1, &s_Data.WhiteTexture));

	delete[] s_Data.QuadBuffer;
}

void BatchRenderer::BeginBatch()
{
	s_Data.QuadBufferPtr = s_Data.QuadBuffer;
}

void BatchRenderer::EndBatch()
{
	GLsizeiptr size = (uint8_t*)s_Data.QuadBufferPtr - (uint8_t*)s_Data.QuadBuffer;
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, s_Data.QuadVB));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_Data.QuadBuffer));
}

void BatchRenderer::Flush()
{
	for (uint32_t i = 0; i < s_Data.TextureSlotIndex; ++i)
	{
		GLCall(glBindTextureUnit(i, s_Data.TextureSlots[i]));
	}

	GLCall(glBindVertexArray(s_Data.QuadVA));
	GLCall(glDrawElements(GL_TRIANGLES, s_Data.IndexCount, GL_UNSIGNED_INT, nullptr));
	++s_Data.BatchRendererStats.DrawCount;

	s_Data.IndexCount = 0;
	s_Data.TextureSlotIndex = 1;
}

void BatchRenderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	if (s_Data.IndexCount >= MaxIndexCount)
	{
		EndBatch();
		Flush();
		BeginBatch();
	}

	float textureIndex = 0.0f;

	s_Data.QuadBufferPtr->Position = { position.x, position.y, 0.0f };
	s_Data.QuadBufferPtr->Color = color;
	s_Data.QuadBufferPtr->TexCoords = { 0.0f, 0.0f };
	s_Data.QuadBufferPtr->TexIndex = textureIndex;
	++s_Data.QuadBufferPtr;

	s_Data.QuadBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
	s_Data.QuadBufferPtr->Color = color;
	s_Data.QuadBufferPtr->TexCoords = { 1.0f, 0.0f };
	s_Data.QuadBufferPtr->TexIndex = textureIndex;
	++s_Data.QuadBufferPtr;

	s_Data.QuadBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
	s_Data.QuadBufferPtr->Color = color;
	s_Data.QuadBufferPtr->TexCoords = { 1.0f, 1.0f };
	s_Data.QuadBufferPtr->TexIndex = textureIndex;
	++s_Data.QuadBufferPtr;

	s_Data.QuadBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
	s_Data.QuadBufferPtr->Color = color;
	s_Data.QuadBufferPtr->TexCoords = { 0.0f, 1.0f };
	s_Data.QuadBufferPtr->TexIndex = textureIndex;
	++s_Data.QuadBufferPtr;

	s_Data.IndexCount += 6;
	++s_Data.BatchRendererStats.QuadCount;
}

void BatchRenderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t textureID)
{
	if (s_Data.IndexCount >= MaxIndexCount || s_Data.TextureSlotIndex > 31)
	{
		EndBatch();
		Flush();
		BeginBatch();
	}

	constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	float textureIndex = 0.0f;

	//if texture was already used for another quad, then grabs it from TextureSlots
	for (uint32_t i = 1; i < s_Data.TextureSlotIndex; ++i)
	{
		if (s_Data.TextureSlots[i] == textureID)
		{
			textureIndex = (float)i;
			break;
		}
	}

	//if texture was not found in TextureSlots, then adds it
	if (textureIndex == 0.0f)
	{
		textureIndex = (float)s_Data.TextureSlotIndex;
		s_Data.TextureSlots[s_Data.TextureSlotIndex] = textureID;
		++s_Data.TextureSlotIndex;
	}

	s_Data.QuadBufferPtr->Position = { position.x, position.y, 0.0f };
	s_Data.QuadBufferPtr->Color = color;
	s_Data.QuadBufferPtr->TexCoords = { 0.0f, 0.0f };
	s_Data.QuadBufferPtr->TexIndex = textureIndex;
	++s_Data.QuadBufferPtr;

	s_Data.QuadBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
	s_Data.QuadBufferPtr->Color = color;
	s_Data.QuadBufferPtr->TexCoords = { 1.0f, 0.0f };
	s_Data.QuadBufferPtr->TexIndex = textureIndex;
	++s_Data.QuadBufferPtr;

	s_Data.QuadBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
	s_Data.QuadBufferPtr->Color = color;
	s_Data.QuadBufferPtr->TexCoords = { 1.0f, 1.0f };
	s_Data.QuadBufferPtr->TexIndex = textureIndex;
	++s_Data.QuadBufferPtr;

	s_Data.QuadBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
	s_Data.QuadBufferPtr->Color = color;
	s_Data.QuadBufferPtr->TexCoords = { 0.0f, 1.0f };
	s_Data.QuadBufferPtr->TexIndex = textureIndex;
	++s_Data.QuadBufferPtr;

	s_Data.IndexCount += 6;
	++s_Data.BatchRendererStats.QuadCount;
}

const BatchRenderer::Stats& BatchRenderer::GetStats()
{
	return s_Data.BatchRendererStats;
}

void BatchRenderer::ResetStats()
{
	memset(&s_Data.BatchRendererStats, 0, sizeof(Stats));
}