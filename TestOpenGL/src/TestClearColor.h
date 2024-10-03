#pragma once
#include "imgui/imgui.h"
#include "Test.h"

class TestClearColor : public Test
{
public:
	TestClearColor() {}
	~TestClearColor();

	void OnUpdate() override;
	void OnImguiRender() override;
	void OnRender() override;

private:
	ImVec4 m_ClearColor{ 0.2f, 0.3f, 0.8f, 1.0f };
};