#pragma once
#include "Vertex.h"

class VertexBuffer {

private:
	unsigned int m_RendererID;
	const bool m_IsDynamic;

public:
	VertexBuffer(std::vector<Vertex*> vertices, bool isDynamic, int MaxVertices);
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
	void UpdateBufferData(std::vector<Vertex*> vertices);

};