#include "Test.h"
#include "../imgui/imgui.h"

namespace test {
	void Test::ToggleWindowCursor()
	{
		if (m_Window == nullptr)
		{
			std::cout << "ToggleWindowFocus() error: No window to toggle\n";
			return;
		}

		if(glfwGetInputMode(m_Window, GLFW_CURSOR) != GLFW_CURSOR_NORMAL)
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void Test::TogglePolygonMode()
	{
		static GLint polygonMode;
		glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
		if(polygonMode == GL_FILL)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		// draw in wireframe
		else if(polygonMode == GL_LINE)
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);		// draw in points
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		// draw normally

	}

	test::TestMenu::TestMenu(Test*& currentTestPointer)
		: m_CurrentTest(currentTestPointer)
	{

	}

	TestMenu::~TestMenu()
	{
	}

	void test::TestMenu::OnImGuiRender()
	{
		for (auto& test : m_Tests)
		{
			if (ImGui::Button(test.first.c_str()))
				m_CurrentTest = test.second();

		}
	}
}
