#include "BatchCubeRenderTest.h"

#include "../BatchRenderer.h"

#include <GLFW/glfw3.h>

#include "../imgui/imgui.h"

#include "../glm/glm.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "../glm/gtc/matrix_transform.hpp"

#include "../stb_image.h"

namespace test {	
	static float m_Vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	// world space positions of our cubes
	static glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

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
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoords;
		float TexID;
	};

	BatchCubeRenderTest::BatchCubeRenderTest()
		: m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f))
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glEnable(GL_DEPTH_TEST));
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


		m_VertexBuffer->Bind();
		m_VertexBuffer->SetData(&m_Vertices, sizeof(m_Vertices));
		//Setting up offsets
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// position attribute
		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));
		//// color attribute
		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));
		//// texture attribute
		//glEnableVertexAttribArray(2);
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));
		////texture index
		//glEnableVertexAttribArray(3);
		//glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexID));

		m_Shader = std::make_unique<Shader>("res/shaders/CubeRender.shader");
		m_Shader->Bind();

		glUseProgram(m_Shader->GetRendererID());
		auto location = glGetUniformLocation(m_Shader->GetRendererID(), "u_Textures");
		int samplers[32];
		for (int i = 0; i < 32; ++i)
			samplers[i] = i;
		glUniform1iv(location, 32, samplers);

		glBindTextureUnit(1, m_TextureOne);
		glBindTextureUnit(2, m_TextureTwo);
	}

	BatchCubeRenderTest::~BatchCubeRenderTest()
	{

	}

	GLuint BatchCubeRenderTest::LoadTexture(const std::string& texturePath)
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

	static Vertex* CreateVertex(Vertex* target, float size, glm::vec3 position, glm::vec2 texCoords, float TextureID)
	{
		target->Position = position;
		target->Color = glm::vec4(0.0f, 0.18f, 0.6f, 1.0f);
		target->TexCoords = { 0.0f, 0.0f };
		target->TexID = TextureID;
		++target;

		return target;
	}

	static Vertex* CreateQuad(Vertex* target, float size, float x, float y, float TextureID)
	{
		target->Position = { x, y, 0.0f };
		target->Color = { 0.0f, 0.18f, 0.0f, 1.0f };
		target->TexCoords = { 0.0f, 0.0f };
		target->TexID = TextureID;
		++target;

		target->Position = { x + size, y, 0.0f };
		target->Color = { 0.0f, 0.18f, 0.0f, 1.0f };
		target->TexCoords = { 1.0f, 0.0f };
		target->TexID = TextureID;
		++target;

		target->Position = { x + size, y + size, 0.0f };
		target->Color = { 0.0f, 0.18f, 0.0f, 1.0f };
		target->TexCoords = { 1.0f, 1.0f };
		target->TexID = TextureID;
		++target;

		target->Position = { x, y + size, 0.0f };
		target->Color = { 0.0f, 0.18f, 0.0f, 1.0f };
		target->TexCoords = { 0.0f, 1.0f };
		target->TexID = TextureID;
		++target;

		return target;
	}

	void BatchCubeRenderTest::OnUpdate(float deltaTime)
	{
	}

	void BatchCubeRenderTest::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); // also clear the depth buffer now!

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, m_TextureOne);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, m_TextureTwo);

		auto texIDLoc = glGetUniformLocation(m_Shader->GetRendererID(), "texIndex");
		glUniform1f(texIDLoc, 0.0f);		//1.0f or 2.0f for textured

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));	//star wars text inclination
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

		glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 0.1f, 100.0f);
		//projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);

		// pass transformation matrices to the shader
		m_Shader->SetUniformMat4f("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		m_Shader->SetUniformMat4f("view", view);

		//m_Shader->Bind();

		Renderer renderer;
		//for (unsigned int i = 0; i < 10; i++)
		{
			m_Shader->SetUniformMat4f("model", model);


			/*int modelLoc = glGetUniformLocation(m_Shader->GetRendererID(), "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			auto viewloc = glGetUniformLocation(m_Shader->GetRendererID(), "view");
			glUniformMatrix4fv(viewloc, 1, GL_FALSE, glm::value_ptr(view));
			auto projloc = glGetUniformLocation(m_Shader->GetRendererID(), "projection");
			glUniformMatrix4fv(projloc, 1, GL_FALSE, glm::value_ptr(projection));*/


			//renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		}
	}


	void BatchCubeRenderTest::OnImGuiRender()
	{
		ImGui::DragFloat2("Quad0", m_QOnePos);
		ImGui::DragFloat2("Quad1", m_QTwoPos);
		ImGui::Text("Application Average %.3f ms/frame (%.1f FPS)", 1000 / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}