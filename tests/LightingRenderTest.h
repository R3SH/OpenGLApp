#pragma once

#include <memory>
#include <array>

#include "Test.h"

#include "../Texture.h"
#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"

#include "../Camera.h"

//#include <GLCore.h>
//#include <GLCoreUtils.h>

//using namespace GLCore;

namespace test {
	class LightingRenderTest :
		public Test
	{
	public:
		LightingRenderTest();
		~LightingRenderTest();


		static GLuint LoadTexture(const std::string& texturePath);
		void Init();
		//void OnEvent(Event& event);
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

		void ProcessInput();
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	private:
		std::unique_ptr<VertexArray> m_VertexArray;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Shader> m_LightCubeShader;
		std::unique_ptr<Texture> m_Texture;

		//OrthographicCameraController m_CameraController;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_TranslationA, m_TranslationB;

		static const size_t MaxQuadCount = 1000;					//Max n of quads to render
		static const size_t MaxVertexCount = MaxQuadCount * 4;		//Max n of vertex to render
		static const size_t MaxIndexCount = MaxQuadCount * 6;		//Max n of indexs to render

		GLuint m_TextureOne, m_TextureTwo;
		float m_QOnePos[2] = { 0.0f, 0.0f }, m_QTwoPos[2] = { 5.0f, -5.0f };
		float m_TextureID = 1.0f;

		// timing
		float m_DeltaTime = 0.0f;	// time between current frame and last frame
		float m_LastFrame = 0.0f;

		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::vec3* m_LightPos;
	};
}

