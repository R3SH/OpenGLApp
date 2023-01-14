#include "BatchRendererRenderTest.h"

#include "../BatchRenderer.h"
#include "../imgui/imgui.h"

#include "../glm/glm.hpp"
#include "../glm/gtc/type_ptr.hpp"
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

	BatchRendererRenderTest::BatchRendererRenderTest()
		: m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f))/*,
		m_CameraController(16.0f / 9.0f)*/
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_Shader = std::make_unique<Shader>("res/shaders/BatchRenderer.shader");
		m_Shader->Bind();

		glUseProgram(m_Shader->GetRendererID());
		auto location = glGetUniformLocation(m_Shader->GetRendererID(), "u_Textures");
		int samplers[32];
		for (int i = 0; i < 32; ++i)
			samplers[i] = i;
		glUniform1iv(location, 32, samplers);
		
		BatchRenderer::Init();

		m_TextureOne = LoadTexture("res/textures/chernoLogo.png");
		m_TextureTwo = LoadTexture("res/textures/menubg.png");
	}

	BatchRendererRenderTest::~BatchRendererRenderTest()
	{
		BatchRenderer::Shutdown();
	}

	static void SetUniformMat4(uint32_t shader, const char* name, const glm::mat4& matrix)
	{
		int loc = glGetUniformLocation(shader, name);
		GLCall(glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix)));
	}

	GLuint BatchRendererRenderTest::LoadTexture(const std::string& texturePath)
	{
		int width, height, bits;

		stbi_set_flip_vertically_on_load(true);
		auto* pixels = stbi_load(texturePath.c_str(), &width, &height, &bits, STBI_rgb);
		if (pixels == nullptr)
		{
			std::cout << "Failed to load texture\n";
			return -1;
		}
		GLuint textureID;
		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &textureID));
		GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels));

		stbi_image_free(pixels);

		return textureID;
	}

	/*void BatchRendererRenderTest::OnEvent(Event& event)
	{
		m_CameraController.OnEvent(event);

		if (event.GetEventType() == EventType::WindowResize)
		{
			WindowResizeEvent& e = (WindowResizeEvent&)event;
			GLCall(glViewport(0, 0, e.GetWidth(), e.GetHeight()));
		}
	}*/

	static Vertex* CreateQuad(Vertex* target, float size, float x, float y, float TextureID)
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

	void BatchRendererRenderTest::OnUpdate(float deltaTime)
	{
	}

	void BatchRendererRenderTest::OnRender()
	{
		//m_CameraController.OnUpdate(1);

		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		glUseProgram(m_Shader->GetRendererID());

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 view = glm::mat4(1.0f);
		// note that we're translating the scene in the reverse direction of where we want to move
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 0.1f, 100.0f);
		int modelLoc = glGetUniformLocation(m_Shader->GetRendererID(), "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		auto viewloc = glGetUniformLocation(m_Shader->GetRendererID(), "view");
		glUniformMatrix4fv(viewloc, 1, GL_FALSE, glm::value_ptr(view));
		auto projloc = glGetUniformLocation(m_Shader->GetRendererID(), "projection");
		glUniformMatrix4fv(projloc, 1, GL_FALSE, glm::value_ptr(projection));

		/*auto vp = m_CameraController.GetCamera().GetViewProjectionMatrix();
		SetUniformMat4(m_Shader->GetRendererID(), "u_ViewProj", vp);
		SetUniformMat4(m_Shader->GetRendererID(), "u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));*/

		BatchRenderer::ResetStats();
		BatchRenderer::BeginBatch();

		for (float i = -10.0f; i < 10.0f; i += 0.25f)
		{
			for (float j = -10.0f; j < 10.0f; j += 0.25f)
			{
				//glm::vec4 color = { (j + 10) / 20.f, 0.2f, (i + 10) / 20.0f, 1.0f };
				GLuint tex = (int)(i + j) % 2 == 0 ? m_TextureOne : m_TextureTwo;
				//BatchRenderer::DrawQuad({ i, j }, { 1.0f, 1.0f }, color);
				BatchRenderer::DrawQuad({ i, j }, { 0.2f, 0.2f }, tex);
			}
		}

		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 5; ++j)
			{
				GLuint tex = (i + j) % 2 == 0 ? m_TextureOne : m_TextureTwo;
				BatchRenderer::DrawQuad({ i, j }, { 1.0f, 1.0f }, tex);
			}
		}

		BatchRenderer::DrawQuad({ m_QOnePos[0], m_QOnePos[1] }, { 1.0f, 1.0f }, m_TextureOne);
		BatchRenderer::DrawQuad({ m_QTwoPos[0], m_QTwoPos[1] }, { 1.0f, 1.0f }, m_TextureTwo);

		BatchRenderer::Flush();
	}

	void BatchRendererRenderTest::OnImGuiRender()
	{
		ImGui::DragFloat2("Quad0", m_QOnePos);
		ImGui::DragFloat2("Quad1", m_QTwoPos);
		ImGui::Text("Application Average %.3f ms/frame (%.1f FPS)", 1000 / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Rendered Quads: %d", BatchRenderer::GetStats().QuadCount);
		ImGui::Text("Rendered Quads: %d", BatchRenderer::GetStats().DrawCount);
	}
}
