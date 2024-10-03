#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "TestClearColor.h"


void TestClearColor::OnUpdate()
{

}

void TestClearColor::OnImguiRender()
{
	ImGui::ColorEdit4("Color", &m_ClearColor.x);
}

void TestClearColor::OnRender()
{
	glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w);
	glClear(GL_COLOR_BUFFER_BIT);
}

TestClearColor::~TestClearColor()
{

}