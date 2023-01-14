#include "DynamicBatchIBTest.h"

#include "../Renderer.h"
#include "../imgui/imgui.h"

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"

#include "../stb_image.h"

namespace test {
	struct Vec2
	{
		float x, y;
	};

	struct Vec3
	{
		float x, y, z;
	};

	struct Vec4
	{
		float x, y, z, w;
	};

	struct Vertex
	{
		Vec3 Position;
		Vec4 Color;
		Vec2 TexCoords;
		float TexID;
	};

	DynamicBatchIBTest::DynamicBatchIBTest()
		: m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f))
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VertexArray = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, MaxVertexCount * sizeof(Vertex));
		VertexBufferLayout layout;
		m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

		/*unsigned int indices[] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};*/

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

		m_TextureOne = LoadTexture("res/textures/chernoLogo.png");
		m_TextureTwo = LoadTexture("res/textures/menubg.png");

		//Setting up offsets
		// position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));
		// color attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));
		// texture attribute
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));
		//texture index
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexID));

		m_Shader = std::make_unique<Shader>("res/shaders/BatchRend.shader");
		m_Shader->Bind();

		glUseProgram(m_Shader->GetRendererID());
		auto location = glGetUniformLocation(m_Shader->GetRendererID(), "u_Textures");
		int samplers[2] = { 0, 1 };
		glUniform1iv(location, 2, samplers);
	}

	DynamicBatchIBTest::~DynamicBatchIBTest()
	{

	}

	GLuint DynamicBatchIBTest::LoadTexture(const std::string& texturePath)
	{
		int width, height, bits;

		stbi_set_flip_vertically_on_load(true);
		auto* pixels = stbi_load(texturePath.c_str(), &width, &height, &bits, STBI_rgb);
		GLuint textureID;
		glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

		stbi_image_free(pixels);

		return textureID;
	}

	static Vertex*  CreateQuad(Vertex* target, float size, float x, float y, float TextureID)
	{
		target->Position = { x, y, 0.0f };
		target->Color = { 0.0f, 0.18f, 0.6f };
		target->TexCoords = { 0.0f, 0.0f };
		target->TexID = TextureID;
		++target;

		target->Position = { x + size, y, 0.0f };
		target->Color = { 0.0f, 0.18f, 0.6f };
		target->TexCoords = { 1.0f, 0.0f };
		target->TexID = TextureID;
		++target;

		target->Position = { x + size, y + size, 0.0f };
		target->Color = { 0.0f, 0.18f, 0.6f };
		target->TexCoords = { 1.0f, 1.0f };
		target->TexID = TextureID;
		++target;

		target->Position = { x, y + size, 0.0f };
		target->Color = { 0.0f, 0.18f, 0.6f };
		target->TexCoords = { 0.0f, 1.0f };
		target->TexID = TextureID;
		++target;

		return target;
	}

	void DynamicBatchIBTest::OnUpdate(float deltaTime)
	{
	}

	void DynamicBatchIBTest::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		/*auto quad0 = CreateQuad(100, m_QOnePos[0], m_QOnePos[1], 0.0f);
		auto quad1 = CreateQuad(100, m_QTwoPos[0], m_QTwoPos[1], 1.0f);*/
		uint32_t indexCount = 0;
		std::array<Vertex, 1000> verticies;
		Vertex* vbuffer = verticies.data();
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 5; ++j)
			{
				vbuffer = CreateQuad(vbuffer, 100, j * 100, i * 100, (i + j) % 2);
				indexCount += 6;
			}
		}

		vbuffer = CreateQuad(vbuffer, 100, m_QOnePos[0], m_QOnePos[1], 0.0f);
		indexCount += 6;

		m_VertexBuffer->Bind();
		m_VertexBuffer->SetSubData(verticies.data(), verticies.size() * sizeof(Vertex), 0);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, indexCount);

		Renderer renderer;

		{
			//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			glBindTextureUnit(0, m_TextureOne);
			glBindTextureUnit(1, m_TextureTwo);
			renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
		}
	}

	void DynamicBatchIBTest::OnImGuiRender()
	{
		ImGui::DragFloat2("Quad0", m_QOnePos);
		ImGui::Text("Application Average %.3f ms/frame (%.1f FPS)", 1000 / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}