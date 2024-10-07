#include "Test.h"
#include "imgui/imgui.h"

TestMenu::TestMenu(Test*& currentTestPointer)
	: m_CurrentTest(currentTestPointer)
{

}

TestMenu::~TestMenu()
{

}

void TestMenu::OnImguiRender()
{
	for (auto& tests : m_Tests)
	{
		if (ImGui::Button(tests.first.c_str()))
			m_CurrentTest = tests.second();
	}
}

