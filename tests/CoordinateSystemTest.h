#pragma once

#include <memory>

#include "Test.h"

#include "../Texture.h"
#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"

namespace test {

	struct TextureParams
	{
		int width, height, bits;
	};

	class CoordinateSystemTest : public Test
	{
	public:
		CoordinateSystemTest();
		~CoordinateSystemTest();


		static GLuint LoadTexture(const std::string& texturePath, TextureParams& tParams);
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<VertexArray> m_VertexArray;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_TranslationA, m_TranslationB;

		GLuint m_TextureOne, m_TextureTwo;
		TextureParams m_TextureOneParams, m_TextureTwoParams;
	};
}