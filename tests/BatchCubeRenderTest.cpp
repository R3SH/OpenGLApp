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
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f
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
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VertexArray = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, sizeof(m_Vertices));
		VertexBufferLayout layout;
		m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		//color attrib
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
		glEnableVertexAttribArray(2);

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

		m_MouseLastX = SCR_WIDTH / 2.0f;
		m_MouseLastY = SCR_HEIGHT / 2.0f;
		m_FirstMouse = true;
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

	void BatchCubeRenderTest::Init()
	{


		//glfwMakeContextCurrent(m_Window);
		//glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
		/*glfwSetCursorPosCallback(m_Window, mouse_callback);
		glfwSetScrollCallback(m_Window, scroll_callback);

		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);*/
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
		float currentFrame = static_cast<float>(glfwGetTime());
		m_DeltaTime = currentFrame - m_LastFrame;
		m_LastFrame = currentFrame;
		if (glfwGetInputMode(m_Window, GLFW_CURSOR) != GLFW_CURSOR_NORMAL)
			glfwSetCursorPosCallback(m_Window, mouse_callback);
		glfwSetScrollCallback(m_Window, scroll_callback);
		ProcessInput();
	}

	void BatchCubeRenderTest::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); // also clear the depth buffer now!

		auto texIDLoc = glGetUniformLocation(m_Shader->GetRendererID(), "texIndex");
		glUniform1f(texIDLoc, m_TextureID);		//1.0f or 2.0f for textured cubes

		glm::mat4 view = m_Camera.GetViewMatrix(); // make sure to initialize matrix to identity matrix first
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		glm::mat4 projection = glm::perspective(glm::radians(m_Camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		//glm::mat4 projection = glm::mat4(1.0f);
		//projection = glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 0.1f, 100.0f);

		// pass transformation matrices to the shader
		m_Shader->SetUniformMat4f("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		m_Shader->SetUniformMat4f("view", view);

		Renderer renderer;
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			m_Shader->SetUniformMat4f("model", model);

			GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		}
	}

	void BatchCubeRenderTest::OnImGuiRender()
	{
		ImGui::InputFloat("TextureID:", &m_TextureID);
		ImGui::Text("Application Average %.3f ms/frame (%.1f FPS)", 1000 / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Press LCTRL to toggle cursor & mouse control");
	}

	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	// ---------------------------------------------------------------------------------------------
	void BatchCubeRenderTest::framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}

	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
	// ---------------------------------------------------------------------------------------------------------
	void BatchCubeRenderTest::ProcessInput()
	{
		if (glfwGetKey(this->m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(this->m_Window, true);

		if (glfwGetKey(this->m_Window, GLFW_KEY_W) == GLFW_PRESS)
			m_Camera.ProcessKeyboard(FORWARD, m_DeltaTime);
		if (glfwGetKey(this->m_Window, GLFW_KEY_S) == GLFW_PRESS)
			m_Camera.ProcessKeyboard(BACKWARD, m_DeltaTime);
		if (glfwGetKey(this->m_Window, GLFW_KEY_A) == GLFW_PRESS)
			m_Camera.ProcessKeyboard(LEFT, m_DeltaTime);
		if (glfwGetKey(this->m_Window, GLFW_KEY_D) == GLFW_PRESS)
			m_Camera.ProcessKeyboard(RIGHT, m_DeltaTime);
		if (glfwGetKey(this->m_Window, GLFW_KEY_SPACE) == GLFW_PRESS)
			m_TextureID = (float)((int)glfwGetTime() % 3);
		if (glfwGetKey(this->m_Window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			ToggleWindowCursor();
	}

	// glfw: whenever the mouse moves, this callback is called
	// -------------------------------------------------------
	void BatchCubeRenderTest::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
	{
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);

		if (m_FirstMouse)
		{
			m_MouseLastX = xpos;
			m_MouseLastY = ypos;
			m_FirstMouse = false;
		}

		float xoffset = xpos - m_MouseLastX;
		float yoffset = m_MouseLastY - ypos; // reversed since y-coordinates go from bottom to top

		m_MouseLastX = xpos;
		m_MouseLastY = ypos;

		m_Camera.ProcessMouseMovement(xoffset, yoffset);
	}

	// glfw: whenever the mouse scroll wheel scrolls, this callback is called
	// ----------------------------------------------------------------------
	void BatchCubeRenderTest::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		m_Camera.ProcessMouseScroll(static_cast<float>(yoffset));
	}
}