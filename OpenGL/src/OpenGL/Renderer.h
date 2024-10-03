#pragma once

#include <GL/glew.h>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if(!(x)) __debugbreak();

class Renderer
{
public:
	void Clear();
	void DrawQuad(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void DrawTriangle(const VertexArray& va, const VertexBuffer& vb, const Shader& shader) const;
};