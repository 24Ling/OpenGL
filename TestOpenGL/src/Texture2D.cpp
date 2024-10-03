#include "Texture2D.h"

#include "imgui/imgui.h"

#include "OpenGL/Renderer.h"
#include "OpenGL/IndexBuffer.h"
#include "OpenGL/VertexBuffer.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/VertexBufferLayout.h"
#include "OpenGL/Shader.h"
#include "OpenGL/Texture.h"

Texture2D::Texture2D()
	:m_Proj(glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f)),
	m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
{
	m_Translation.insert({ "Asprose",	{ 200,  200, 0 } });
	m_Translation.insert({ "Blue",		{ 600,  200, 0 } });
	m_Translation.insert({ "Cpp",		{ 1000, 200, 0 } });
	//m_Translation.insert({ "KDA",		{ 200,  600, 0 } });
	m_Translation.insert({ "Ling",		{ 600,  600, 0 } });
	m_Translation.insert({ "Wang",		{ 1000, 600, 0 } });
	m_Translation.insert({ "Syndra",	{ 1400, 600, 0 } });

	float positions[] = {
		-200.0f, -200.0f, 0.0f, 0.0f,  // 0
		 200.0f, -200.0f, 1.0f, 0.0f,  // 1
		 200.0f,  200.0f, 1.0f, 1.0f,  // 2
		-200.0f,  200.0f, 0.0f, 1.0f   // 3
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	m_VertexArray = std::make_shared<VertexArray>();
	m_Vertexbuffer = std::make_shared<VertexBuffer>(positions, 4 * 4 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);

	m_VertexArray->AddBuffer(*m_Vertexbuffer, layout);
	m_Indexbuffer = std::make_shared<IndexBuffer>(indices, 6 * sizeof(unsigned int));

	m_Texture.push_back(std::make_shared<Texture>("resource/textures/Asprose.png"));
	m_Texture.push_back(std::make_shared<Texture>("resource/textures/Blue.png"));
	m_Texture.push_back(std::make_shared<Texture>("resource/textures/Cpp.png"));
	//m_Texture.push_back(std::make_shared<Texture>("resource/textures/KDA.png"));
	m_Texture.push_back(std::make_shared<Texture>("resource/textures/Ling.png"));
	m_Texture.push_back(std::make_shared<Texture>("resource/textures/Wang.png"));
	m_Texture.push_back(std::make_shared<Texture>("resource/textures/Syndra.png"));

	for (int i = 0; i < m_Texture.size(); i++)
		m_Texture[i]->Bind(i);

	m_Shader = std::make_shared<Shader>("resource/Shaders/Texture2D.shader");


}

Texture2D::~Texture2D()
{

}

void Texture2D::OnUpdate()
{
	if (events.EventCallBack(KeyType::HAZEL_KEY_Q))
		for (auto& translations : m_Translation)
			translations.second[1] += 5;
	else if (events.EventCallBack(KeyType::HAZEL_KEY_E))
		for (auto& translations : m_Translation)
			translations.second[1] -= 5;
}

void Texture2D::OnImguiRender()
{
	ImGui::SliderFloat("scale", &m_Scale, 0.0f, 1.0f);

	for (auto& rotation : m_Rotation)
	{
		std::string name = "Rotation " + rotation.first + ":";
		ImGui::SliderFloat(name.c_str(), &rotation.second, 0.0f, 360.0f);
	}
}

void Texture2D::OnRender()
{
	
	// Renderer Picture
	{
		unsigned int slot = 0;
		for (auto& translations : m_Translation)
		{

			m_Shader->SetUniform1i("u_Texture", slot);
			slot++;

			glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
			m_Shader->SetUniform4f("ourcolor", color);

			//static bool Key = true;

			glm::mat4 model = glm::translate(glm::mat4(1.0f), translations.second);
			model = glm::rotate(model, glm::radians(m_Rotation["x"]), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(m_Rotation["y"]), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(m_Rotation["z"]), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(m_Scale));

			m_View = glm::lookAt(glm::vec3(1920.0f / 2.0f, 1080.0f / 2.0f, 900.0f), glm::vec3(1920.0f / 2.0f, 1080.0f / 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			m_Proj = glm::perspective(glm::radians(60.0f), 1920.0f / 1080.0f, 0.1f, 5000.0f);

			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.DrawQuad(*m_VertexArray, *m_Indexbuffer, *m_Shader);
		}

	}
}
