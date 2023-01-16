#pragma once

#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <functional>

#include "../Camera.h";

namespace test {

	// settings
	static const unsigned int SCR_WIDTH = 960;
	static const unsigned int SCR_HEIGHT = 540;
	

	static Camera m_Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	static float m_MouseLastX;
	static float m_MouseLastY;
	static bool m_FirstMouse;

	class Test
	{
	public:
		Test() {}
		Test(GLFWwindow* window) { m_Window = window; }
		virtual ~Test() {}

		virtual void Init() {}
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}

		virtual void GetWindow(GLFWwindow* window) { m_Window = window; }
		
		GLFWwindow* m_Window = nullptr;
	};

	class TestMenu : public Test
	{
	public:	
		TestMenu(Test*& currentTestPointer);
		~TestMenu();

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering test " << name << std::endl;

			m_Tests.push_back(std::make_pair(name, []() {return new T; }));
		}
	private:
		Test*& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
	};

}