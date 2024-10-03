#pragma once
#include <array>

#include "Test.h"
#include "OpenGL/Renderer.h"
#include "OpenGL/VertexBuffer.h"
#include "OpenGL/IndexBuffer.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/VertexBufferLayout.h"
#include "OpenGL/Shader.h"

class TestTriangle : public Test
{
public:
	TestTriangle();
	~TestTriangle();


	void OnUpdate() override;
	void OnImguiRender() override;
	void OnRender() override;
private:
	std::shared_ptr<Shader> m_Shader;
	std::vector<std::shared_ptr<VertexArray>> m_VAO;
	std::vector<std::shared_ptr<VertexBuffer>> m_VBO;
	std::shared_ptr<IndexBuffer> m_Indexbuffer;

	std::vector<std::array<float, 21>> m_Positions;
	std::array<float, 28> vertices_quad;

};