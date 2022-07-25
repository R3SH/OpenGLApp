#include "BatchColorTest.h"

#include "../Renderer.h"
#include "../imgui/imgui.h"

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"

namespace test {
	BatchColorTest::BatchColorTest()
		: m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f))
	{
		float positions[] = {
			-150.0f,  -50.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f,		//0
			 -50.0f,  -50.0f, 1.0f, 0.18f, 0.6f, 0.96f, 1.0f,		//1
			 -50.0f,   50.0f, 1.0f, 0.18f, 0.6f, 0.96f, 1.0f,		//2
			-150.0f,   50.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f,		//3

			  50.0f, -50.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f,		//4
			 150.0f, -50.0f, 1.0f, 1.0f, 0.93f, 0.24f, 1.0f,		//5
			 150.0f,  50.0f, 1.0f, 1.0f, 0.93f, 0.24f, 1.0f,		//6
			  50.0f,  50.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f			//7
		};

		unsigned int indices[] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VertexArray = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 2 * (4 * 7 * sizeof(float)));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		layout.Push<float>(3);	//
		layout.Push<float>(1);	//

		m_VertexArray->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);		//6

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		m_Shader = std::make_unique<Shader>("res/shaders/BatchRend.shader");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
		m_Texture = std::make_unique<Texture>("res/textures/chernoLogo.png");
		m_Shader->SetUniform1i("u_Texture", 0);
	}

	BatchColorTest::~BatchColorTest()
	{

	}

	void BatchColorTest::OnUpdate(float deltaTime)
	{
	}

	void BatchColorTest::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		m_Texture->Bind();

		//{
		//	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
		//	glm::mat4 mvp = m_Proj * m_View * model;
		//	m_Shader->Bind();
		//	m_Shader->SetUniformMat4f("u_MVP", mvp);
		//	renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
		//}
		{
			//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
		}
	}

	void BatchColorTest::OnImGuiRender()
	{
		ImGui::SliderFloat3("TranslationA", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::Text("Application Average %.3f ms/frame (%.1f FPS)", 1000 / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}