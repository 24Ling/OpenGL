#include "Renderer.h"
#include "VertexBufferLayout.h"

void Renderer::Clear()
{

}

void Renderer::DrawQuad(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::DrawTriangle(const VertexArray& va, const VertexBuffer& vb, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	vb.Bind();

	glDrawArrays(GL_TRIANGLES, 0, 6);
}