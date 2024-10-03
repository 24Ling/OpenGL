#pragma once
#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const;
	void Bind() const;
	void UnBind() const;

private:
	uint32_t m_RendererID;
	uint32_t m_Position = 0;
};