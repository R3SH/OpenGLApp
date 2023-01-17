#include "ModelRenderTest.h"

#include "../BatchRenderer.h"

#include <GLFW/glfw3.h>

#include "../imgui/imgui.h"

#include "../glm/glm.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "../glm/gtc/matrix_transform.hpp"

#include "../stb_image.h"

// to get current dir 
#include <direct.h>

namespace test {
	ModelRenderTest::ModelRenderTest()
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		stbi_set_flip_vertically_on_load(true);

		//Shaders init
		m_Shader = std::make_unique<Shader>("res/shaders/ModelRender.shader");
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

		char buff[256];
		_getcwd(buff, 256);
		std::string modelSrc(buff);
		//modelSrc += "\\res\\models\\AK74M.obj";
		//modelSrc += "\\res\\models\\AK-74M.blend";
		m_Model = new Model(modelSrc + "\\res\\models\\AK74M.obj");
		m_GlockModel = new Model(modelSrc + "\\res\\models\\Glock17\\Glock17.obj");
		m_BackpackModel = new Model(modelSrc + "\\res\\models\\backpack\\backpack.obj");
		//m_Model = new Model("res/models/AK74M.obj");
	}

	ModelRenderTest::~ModelRenderTest()
	{

	}

	GLuint ModelRenderTest::LoadTexture(const std::string& texturePath)
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

	void ModelRenderTest::Init()
	{
	}

	void ModelRenderTest::OnUpdate(float deltaTime)
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		m_DeltaTime = currentFrame - m_LastFrame;
		m_LastFrame = currentFrame;

		if (glfwGetInputMode(m_Window, GLFW_CURSOR) != GLFW_CURSOR_NORMAL)
			glfwSetCursorPosCallback(m_Window, mouse_callback);
		glfwSetScrollCallback(m_Window, scroll_callback);
		ProcessInput();
	}

	void ModelRenderTest::OnRender()
	{
		GLCall(glClearColor(1.0f, 0.64f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); // also clear the depth buffer now!


		glm::mat4 view = m_Camera.GetViewMatrix(); // make sure to initialize matrix to identity matrix first

		glm::mat4 projection = glm::perspective(glm::radians(m_Camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		//DONT FORGET TO BIND SHADER EACH FRAME IF MULTIPLE SHADERS
		m_Shader->Bind();
		// pass transformation matrices to the shader
		m_Shader->SetUniformMat4f("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		m_Shader->SetUniformMat4f("view", view);

		//rendering loaded model AK74M
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));	// translate it down so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));		// it's a bit too big for our scene, so scale it down
		m_Shader->SetUniformMat4f("model", model);
		m_Model->Draw(*m_Shader);

		//GLOCK17
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));	// translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));		// it's a bit too big for our scene, so scale it down
		m_Shader->SetUniformMat4f("model", model);
		m_GlockModel->Draw(*m_Shader);

		//BACKPACK
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));	// translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));		// it's a bit too big for our scene, so scale it down
		m_Shader->SetUniformMat4f("model", model);
		m_BackpackModel->Draw(*m_Shader);
	}

	void ModelRenderTest::OnImGuiRender()
	{
		/*ImGui::SliderFloat3("LightAmbient", &lightAmbient.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("LightDiffuse", &lightDiffuse.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("LightSpecular", &lightSpecular.x, 0.0f, 1.0f);*/
		ImGui::InputFloat("TextureID:", &m_TextureID);
		ImGui::Text("Application Average %.3f ms/frame (%.1f FPS)", 1000 / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Press LCTRL to toggle cursor & mouse control");
		ImGui::Text("Press SPACE to toggle polygon mode");
	}

	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	// ---------------------------------------------------------------------------------------------
	void ModelRenderTest::framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}

	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
	// ---------------------------------------------------------------------------------------------------------
	void ModelRenderTest::ProcessInput()
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
			TogglePolygonMode();
		if (glfwGetKey(this->m_Window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			ToggleWindowCursor();
	}

	// glfw: whenever the mouse moves, this callback is called
	// -------------------------------------------------------
	void ModelRenderTest::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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
	void ModelRenderTest::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		m_Camera.ProcessMouseScroll(static_cast<float>(yoffset));
	}
}