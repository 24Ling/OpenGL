#pragma once
#include <iostream>

class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;


private:
	uint32_t m_RendererID;
};