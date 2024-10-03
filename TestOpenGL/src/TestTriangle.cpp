#include "imgui/imgui.h"
#include "TestTriangle.h"


TestTriangle::TestTriangle()
{
	vertices_quad = {
		-0.5f, -0.5f, 1.0f, 0.5f, 0.2f, 0.7f, 1.0f,
		 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, 1.0f, 1.0f, 0.2f, 0.7f, 1.0f,
		-0.5f,  0.5f, 1.0f, 1.0f, 0.2f, 0.7f, 1.0f
	};

	m_Positions.push_back({
		-0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		 0.0f,  0.5f, 1.0f, 0.0f, 0.2f, 0.3f, 1.0f
		});
	m_Positions.push_back({
		-0.3f, -0.3f, 1.0f, 0.5f, 0.2f, 0.7f, 0.2f,
		 0.8f, -0.3f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		 0.2f,  0.7f, 1.0f, 1.0f, 0.2f, 0.7f, 1.0f
		});

	for (const auto& position : m_Positions)
	{
		m_VAO.push_back(std::make_shared<VertexArray>());
		m_VBO.push_back(std::make_shared<VertexBuffer>(&position[0], 3 * 7 * sizeof(float)));
	}

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(4);

	m_VAO.push_back(std::make_shared<VertexArray>());
	m_VBO.push_back(std::make_shared<VertexBuffer>(&vertices_quad[0], 4 * 7 * sizeof(float)));
	unsigned int indices1[] = {
		0, 1, 2,
		2, 3, 0
	};
	m_Indexbuffer = std::make_shared<IndexBuffer>(indices1, 6 * sizeof(int));


	for (int i = 0; i < m_VAO.size(); i++)
		m_VAO[i]->AddBuffer(*m_VBO[i], layout);

	m_Shader = std::make_shared<Shader>("resource/Shaders/Triangle.shader");
}

void TestTriangle::OnUpdate()
{
	m_VAO.clear();
	m_VBO.clear();

	for (const auto& position : m_Positions)
	{
		m_VAO.push_back(std::make_shared<VertexArray>());
		m_VBO.push_back(std::make_shared<VertexBuffer>(&position[0], 3 * 7 * sizeof(float)));
	}

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(4);

	m_VAO.push_back(std::make_shared<VertexArray>());
	m_VBO.push_back(std::make_shared<VertexBuffer>(&vertices_quad[0], 4 * 7 * sizeof(float)));

	for (int i = 0; i < m_VAO.size(); i++)
		m_VAO[i]->AddBuffer(*m_VBO[i], layout);
}

void TestTriangle::OnImguiRender()
{
	ImGui::ColorEdit4("Triangle A Position 1:", &m_Positions[0][3]);
	ImGui::ColorEdit4("Triangle A Position 2:", &m_Positions[0][10]);
	ImGui::ColorEdit4("Triangle A Position 3:", &m_Positions[0][17]);
	ImGui::ColorEdit4("Triangle B Position 1:", &m_Positions[1][3]);
	ImGui::ColorEdit4("Triangle B Position 2:", &m_Positions[1][10]);
	ImGui::ColorEdit4("Triangle B Position 3:", &m_Positions[1][17]);
	ImGui::ColorEdit4("Quad Position1:", &vertices_quad[3]);
	ImGui::ColorEdit4("Quad Position2:", &vertices_quad[10]);
	ImGui::ColorEdit4("Quad Position3:", &vertices_quad[17]);
	ImGui::ColorEdit4("Quad Position4:", &vertices_quad[24]);

}

void TestTriangle::OnRender()
{
	Renderer renderer;

	for (int i = 0; i < m_VAO.size() - 1; i++)
		renderer.DrawTriangle(*m_VAO[i], *m_VBO[i], *m_Shader);
	renderer.DrawQuad(*m_VAO[2], *m_Indexbuffer, *m_Shader);

}

TestTriangle::~TestTriangle()
{
	m_VAO.clear();
	m_VBO.clear();
}


