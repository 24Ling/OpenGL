#pragma once

#include <string>
#include "include/stb_image.h"


class Texture
{
public:
	Texture(const std::string& path, bool verteical = true);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	int GetWidth()		const	{ return m_Width; }
	int GetHeight()		const	{ return m_Height; }
	int GetBPP()		const	{ return m_BPP; }
	int GetRendererID() const	{ return m_RendererID; }
private:
	uint32_t m_RendererID;
	unsigned char* m_LocalBuffer;
	std::string m_FilePath;
	int m_Width, m_Height, m_BPP;
};