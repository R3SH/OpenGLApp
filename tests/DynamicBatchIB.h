#pragma once

#include <memory>
#include <array>

#include "Test.h"

#include "../Texture.h"
#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"

namespace test {
    class DynamicBatchIB :
        public Test
    {
	public:
		DynamicBatchIB();
		~DynamicBatchIB();


		static GLuint LoadTexture(const std::string& texturePath);
		//static std::array<Vertex, 4> CreateQuad(float x, float y, float TextureID);
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

		static const size_t MaxQuadCount = 1000;		//Max n of quads to render
		static const size_t MaxVertexCount = MaxQuadCount * 4;		//Max n of vertex to render
		static const size_t MaxIndexCount = MaxQuadCount * 6;		//Max n of indexs to render

		uint32_t indices[MaxIndexCount];

		GLuint m_TextureOne, m_TextureTwo;
		float m_QOnePos[2] = { -150.0f, -50.0f }, m_QTwoPos[2] = { 50.0f, -50.0f };
    };

}

