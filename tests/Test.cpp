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
