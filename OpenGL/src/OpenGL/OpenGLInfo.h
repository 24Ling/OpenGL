#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

class OpenGLInfo
{
public:
	OpenGLInfo();
	~OpenGLInfo();

	void InitOpenGL();
	void DestroyOpenGL();
	bool ShowWindow();
	void EnableBlend();
	void PollEvent();
	void ClearColor();

	unsigned int GetWidth() const { return m_Width; }
	unsigned int GetHeight() const { return m_Height; }
	GLFWwindow* GetWindow() { return m_Window; }

private:
	std::string WindowName;
	unsigned int m_Width;
	unsigned int m_Height;

	GLFWwindow* m_Window = nullptr;
};