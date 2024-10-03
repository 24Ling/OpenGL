#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>


class ImGuiLayer
{
public:
	ImGuiLayer() {}
	~ImGuiLayer();
	void InitImGui(GLFWwindow* window);
	void ImGuiRender(GLFWwindow* window);
	void OnImGuiBegin();
	void OnImGuiEnd();
	void ImGuiShowDemo();
	void ImGuiShutdown();

	void ImGuiBegin(std::string name);
	void ImGuiEnd();
	bool ImGuiButton(std::string name);

private:
	bool m_show_demo_window = true;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};