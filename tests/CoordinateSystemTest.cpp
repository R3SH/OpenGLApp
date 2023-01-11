#include "CoordinateSystemTest.h"

#include "../Renderer.h"
#include "../imgui/imgui.h"

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"

#include "../stb_image.h"

namespace test {
	CoordinateSystemTest::CoordinateSystemTest()
		: m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		//m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f))
		m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, 0.1f, 100.0f))
	{
		float positions[] = {
			-150.0f,  -50.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 0.0f, 0.0f,		//0
			 -50.0f,  -50.0f, 1.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 0.0f, 0.0f,		//1
			 -50.0f,   50.0f, 1.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 1.0f, 0.0f,		//2
			-150.0f,   50.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 1.0f, 0.0f,		//3

			  50.0f,  -50.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 0.0f, 1.0f,		//4
			 150.0f,  -50.0f, 1.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 0.0f, 1.0f,		//5
			 150.0f,   50.0f, 1.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 1.0f, 1.0f,		//6
			  50.0f,   50.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 1.0f, 1.0f		//7
		};

		unsigned int indices[] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VertexArray = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 2 * (4 * 10 * sizeof(float)));
		VertexBufferLayout layout;
		//layout.Push<float>(2);
		//layout.Push<float>(2);
		//layout.Push<float>(3);	//
		//layout.Push<float>(2);	//
		//layout.Push<float>(1);	//

		m_VertexArray->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);		//6

		m_TextureOne = LoadTexture("res/textures/chernoLogo.png", m_TextureOneParams);
		m_TextureTwo = LoadTexture("res/textures/menubg.png", m_TextureTwoParams);

		//Setting up offsets
		// position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
		// color attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
		// texture attribute
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(7 * sizeof(float)));
		//texture index
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(9 * sizeof(float)));

		m_Shader = std::make_unique<Shader>("res/shaders/BatchRend.shader");
		m_Shader->Bind();

		glUseProgram(m_Shader->GetRendererID());
		auto location = glGetUniformLocation(m_Shader->GetRendererID(), "u_Textures");
		int samplers[2] = { 0, 1 };
		glUniform1iv(location, 2, samplers);
	}

	CoordinateSystemTest::~CoordinateSystemTest()
	{

	}

	GLuint CoordinateSystemTest::LoadTexture(const std::string& texturePath, TextureParams& tParams)
	{
		

		stbi_set_flip_vertically_on_load(true);
		auto* pixels = stbi_load(texturePath.c_str(), &tParams.width, &tParams.height, &tParams.bits, STBI_rgb);
		GLuint textureID;
		glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, tParams.width, tParams.height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

		stbi_image_free(pixels);

		return textureID;
	}

	void CoordinateSystemTest::OnUpdate(float deltaTime)
	{
	}

	void CoordinateSystemTest::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		{
			//PROBLEM WITH NON DYNAMIC BATCH RENDERING

			//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.3f, 0.5f));
			//m_Proj = glm::perspective(glm::radians(45.0f), (float)glGetTexParameteriv(m_TextureOne, ) / (float)m_Texture->GetHeight(), 0.1f, 100.0f);
			//m_Proj = glm::perspective(glm::radians(45.0f), (float)960 / (float)540, 0.1f, 100.0f);
			//m_Proj = glm::perspective(glm::radians(45.0f), (float)960 / (float)540, -1.0f, 1.0f);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			glBindTextureUnit(0, m_TextureOne);
			glBindTextureUnit(1, m_TextureTwo);
			renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
		}
	}

	void CoordinateSystemTest::OnImGuiRender()
	{
		ImGui::SliderFloat3("TranslationA", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::Text("Application Average %.3f ms/frame (%.1f FPS)", 1000 / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}