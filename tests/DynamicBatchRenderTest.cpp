#include "DynamicBatchRenderTest.h"

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

	DynamicBatchRenderTest::DynamicBatchRenderTest()
		: m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f))
	{

		//float positions[] = {
		//	-150.0f,  -50.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 0.0f, 0.0f,		//0
		//	 -50.0f,  -50.0f, 1.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 0.0f, 0.0f,		//1
		//	 -50.0f,   50.0f, 1.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 1.0f, 0.0f,		//2
		//	-150.0f,   50.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 1.0f, 0.0f,		//3

		//	  50.0f,  -50.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 0.0f, 1.0f,		//4
		//	 150.0f,  -50.0f, 1.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 0.0f, 1.0f,		//5
		//	 150.0f,   50.0f, 1.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 1.0f, 1.0f,		//6
		//	  50.0f,   50.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 1.0f, 1.0f		//7
		//};

		unsigned int indices[] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VertexArray = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, sizeof(Vertex) * 1000);
		VertexBufferLayout layout;
		//layout.Push<float>(2);
		//layout.Push<float>(2);
		//layout.Push<float>(3);	//
		//layout.Push<float>(2);	//
		//layout.Push<float>(1);	//

		m_VertexArray->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);		//6

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

	DynamicBatchRenderTest::~DynamicBatchRenderTest()
	{

	}

	GLuint DynamicBatchRenderTest::LoadTexture(const std::string& texturePath)
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

	static std::array<Vertex, 4> CreateQuad(float size, float x, float y, float TextureID)
	{
		//float size = 1.0f;		//means quad is 1:1

		Vertex v0;
		v0.Position = { x, y, 0.0f };
		v0.Color = { 0.0f, 0.18f, 0.6f };
		v0.TexCoords = { 0.0f, 0.0f };
		v0.TexID = TextureID;

		Vertex v1;
		v1.Position = { x + size, y, 0.0f };
		v1.Color = { 0.0f, 0.18f, 0.6f };
		v1.TexCoords = { 1.0f, 0.0f };
		v1.TexID = TextureID;

		Vertex v2;
		v2.Position = { x + size, y + size, 0.0f };
		v2.Color = { 0.0f, 0.18f, 0.6f };
		v2.TexCoords = { 1.0f, 1.0f };
		v2.TexID = TextureID;

		Vertex v3;
		v3.Position = { x, y + size, 0.0f };
		v3.Color = { 0.0f, 0.18f, 0.6f };
		v3.TexCoords = { 0.0f, 1.0f };
		v3.TexID = TextureID;

		return { v0, v1, v2, v3 };
	}

	void DynamicBatchRenderTest::OnUpdate(float deltaTime)
	{
	}

	void DynamicBatchRenderTest::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		auto quad0 = CreateQuad(100, m_QOnePos[0], m_QOnePos[1], 0.0f);
		auto quad1 = CreateQuad(100, m_QTwoPos[0], m_QTwoPos[1], 1.0f);

		Vertex positions[8];
		memcpy(positions, quad0.data(), quad0.size() * sizeof(Vertex));
		memcpy(positions + quad0.size(), quad1.data(), quad1.size() * sizeof(Vertex));

		m_VertexBuffer->Bind();
		m_VertexBuffer->SetSubData(positions, sizeof(positions), 0);

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

	void DynamicBatchRenderTest::OnImGuiRender()
	{
		ImGui::DragFloat2("Quad0", m_QOnePos);
		ImGui::DragFloat2("Quad1", m_QTwoPos);
		ImGui::Text("Application Average %.3f ms/frame (%.1f FPS)", 1000 / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}