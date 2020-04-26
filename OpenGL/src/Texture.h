#pragma once

#include "OpenGLUtils.h"

class Texture {

public:
	Texture(const std::string& path, const unsigned int slot);
	~Texture();

	// Mobile = 8 slots, PC = 32 slots
	void Bind(unsigned int slot = 0);
	void Unbind();

	inline int GetWidth() { return m_Width; }
	inline int GetHeight() { return m_Height; }

private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
};