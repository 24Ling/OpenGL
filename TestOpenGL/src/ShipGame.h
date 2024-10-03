#pragma once

#include "Test.h"

#include <vector>
#include <functional>
#include <string>
#include <iostream>
#include <unordered_map>
#include <random>
#include "OpenGL/Event.h"

class ShipGame : public Test
{
public:
	ShipGame();
	~ShipGame();


	void OnUpdate() override;
	void OnImguiRender() override;
	void OnRender() override;
private:
	std::shared_ptr<VertexArray> m_VAO;
	std::shared_ptr<VertexBuffer> m_VertexBuffer;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;

	std::shared_ptr<VertexArray> m_P_VAO;
	std::shared_ptr<VertexBuffer> m_P_VertexBuffer;
	std::shared_ptr<IndexBuffer> m_P_IndexBuffer;

	std::shared_ptr<Shader> m_Shader;
	std::vector<std::shared_ptr<Texture>> m_Texture;

	glm::mat4 m_Proj, m_View;
	std::unordered_map<std::string, glm::vec3> m_Translation;
	glm::vec4 color = { 1.0f, 1.0f, 0.0f, 1.0f };
	float rotation = -90.0f;

	Event event;
};