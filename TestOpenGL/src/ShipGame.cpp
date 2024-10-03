
#include "imgui/imgui.h"

#include "OpenGL/Renderer.h"
#include "OpenGL/IndexBuffer.h"
#include "OpenGL/VertexBuffer.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/VertexBufferLayout.h"
#include "OpenGL/Shader.h"
#include "OpenGL/Texture.h"

#include "ShipGame.h"



ShipGame::ShipGame()
	:m_Proj(glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f)),
	m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
{

	for (int i = 0; i < 14; i++)
	{
		std::string triangle1 = "Triangle" + std::to_string(i);

		std::random_device rd;
		std::mt19937 gen(rd());

		std::uniform_real_distribution<> float1Dist(-700.0f, -600.0f);

		double position = float1Dist(gen);
		m_Translation.insert({ triangle1, { i * 192.0f, position, 0.0f} });


		std::string triangle = "Triangle" + std::to_string(i + 14);
		m_Translation.insert({ triangle, { i * 192.0f, position + 1600.0f, 0.0f} });
	}

	float positions[] = {
		-400.0f, -400.0f, 0.0f, 0.0f,  // 0
		 800.0f, -400.0f, 1.0f, 0.0f,  // 1
		 800.0f,  1200.0f, 1.0f, 1.0f,  // 2
		-400.0f,  1200.0f, 0.0f, 1.0f   // 3
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};


	m_VAO = std::make_unique<VertexArray>();
	m_VertexBuffer = std::make_shared<VertexBuffer>(positions, 4 * 4 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);

	m_VAO->AddBuffer(*m_VertexBuffer, layout);
	m_IndexBuffer = std::make_shared<IndexBuffer>(indices, 6 * sizeof(unsigned int));



	m_Texture.push_back(std::make_shared<Texture>("resource/textures/Triangle.png"));
	m_Texture.push_back(std::make_shared<Texture>("resource/textures/Triangle.png", false));
	m_Texture.push_back(std::make_shared<Texture>("resource/textures/Ship.png"));


	m_Translation.insert({ "Ship", { 100.0f, 100.0f, 0.0f } });

	float positions1[] = {
		-16.0f, -16.0f, 0.0f, 0.0f,  // 0
		 16.0f, -16.0f, 1.0f, 0.0f,  // 1
		 16.0f,  16.0f, 1.0f, 1.0f,  // 2
		-16.0f,  16.0f, 0.0f, 1.0f   // 3
	};

	unsigned int indices1[] = {
		1, 2, 0,
		3, 0, 2
	};

	m_P_VAO = std::make_unique<VertexArray>();
	m_P_VertexBuffer = std::make_shared<VertexBuffer>(positions1, 4 * 4 * sizeof(float));

	m_P_VAO->AddBuffer(*m_P_VertexBuffer, layout);
	m_P_IndexBuffer = std::make_shared<IndexBuffer>(indices1, 6 * sizeof(unsigned int));

	for (int i = 0; i < m_Texture.size(); i++)
		m_Texture[i]->Bind(i);

	m_Shader = std::make_shared<Shader>("resource/Shaders/Texture2D.shader");

}

void ShipGame::OnUpdate()
{
	if (event.EventCallBack(KeyType::HAZEL_KEY_SPACE))
	{
		if (rotation < -90.0f)
			rotation = -90.0f;
		rotation += 0.1f;
		m_Translation["Ship"][1] += 0.7;
	}
	else
	{
		rotation -= 0.1f;
		m_Translation["Ship"][1] -= 0.7;
	}


	if (event.EventCallBack(KeyType::HAZEL_KEY_Q))
	{
		// Todo: Control Player move
		m_Translation["Ship"][1] += 1;

		if (m_Translation["Ship"][1] > 540)
			m_Translation["Ship"][1] = -516;
	}

	if (event.EventCallBack(KeyType::HAZEL_KEY_E))
	{
		// Todo: Control Player move
		m_Translation["Ship"][1] -= 1;

		if (m_Translation["Ship"][1] < -532)
			m_Translation["Ship"][1] = 524;
	}

	static bool key = true;
	if (key)
		color[0] -= 0.001f;
	else
		color[0] += 0.001f;

	if (color[0] > 1.0f)
		key = true;

	else if (color[0] < 0.0f)
		key = false;


	for (auto& translations : m_Translation)
	{
		if (translations.first == "Ship")
			continue;
		if (translations.second[0] < -400)
			translations.second[0] = 1920 + 400;
		else
			translations.second[0] -= 1;
	}


}

void ShipGame::OnImguiRender()
{
	int score = 100;
	ImGui::InputInt("Score", &score);
}

void ShipGame::OnRender()
{
	Renderer renderer;

	for (const auto& translations : m_Translation)
	{
		if (translations.first == "Ship")
			continue;

		std::string position = translations.first;

		std::string str = position.substr(8, position.size());
		 
		std::istringstream ss(str);

		int index;
		ss >> index;

		if (index < 14)
			m_Shader->SetUniform1i("u_Texture", 0);
		else
			m_Shader->SetUniform1i("u_Texture", 1);

		m_Shader->SetUniform4f("ourcolor", color);

		glm::mat4 model = glm::translate(glm::mat4(1.0f), translations.second);
		glm::mat4 mvp = m_Proj * m_View * model;
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp);
		renderer.DrawQuad(*m_VAO, *m_IndexBuffer, *m_Shader);
	}


	// Renderer Ship
	{
		m_Shader->SetUniform1i("u_Texture", 2);
		m_Shader->SetUniform4f("ourcolor", { 1.0f, 1.0f, 1.0f, 1.0f });

		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation["Ship"]);
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -800.0f));

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 1000.0f);

		glm::mat4 mvp = projection * view * model;
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp);
		renderer.DrawQuad(*m_P_VAO, *m_P_IndexBuffer, *m_Shader);
	}


}


ShipGame::~ShipGame()
{
	m_Texture.clear();
}

