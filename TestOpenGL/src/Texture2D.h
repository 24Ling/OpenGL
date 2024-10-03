#pragma once
#include "Test.h"

#include "OpenGL/Renderer.h"
#include "OpenGL/Texture.h"
#include "OpenGL/Event.h"
#include <vector>
#include <functional>
#include <string>
#include <iostream>
#include <unordered_map>
#include <random>


class Texture2D : public Test
{
public:
	Texture2D();
	~Texture2D();


	void OnUpdate() override;
	void OnImguiRender() override;
	void OnRender() override;

private:

	std::shared_ptr<VertexArray> m_VertexArray;
	std::shared_ptr<VertexBuffer> m_Vertexbuffer;
	std::shared_ptr<IndexBuffer> m_Indexbuffer;

	std::shared_ptr<Shader> m_Shader;
	std::vector<std::shared_ptr<Texture>> m_Texture;

	glm::mat4 m_Proj, m_View;
	std::unordered_map<std::string, glm::vec3> m_Translation;

	std::unordered_map<std::string, float> m_Rotation = { {"x", 0.0f}, {"y", 0.0f}, {"z", 0.0f} };
	float m_Scale = 1.0f;

	Renderer renderer;
	Event events;
};