#pragma once

#include <iostream>

class IndexBuffer
{
public:
	IndexBuffer(const void* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() { return m_Count; }

private:
	uint32_t m_RendererID;
	uint32_t m_Count;
};