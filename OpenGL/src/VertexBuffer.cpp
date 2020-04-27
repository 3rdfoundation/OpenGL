#include "opengl.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(std::vector<Vertex*> vertices, bool isDynamic, int MaxVertices) 
	: m_IsDynamic(isDynamic) {

	GLCALL(glGenBuffers(1, &m_RendererID));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));

	// Calculate size
	unsigned int size = MaxVertices * Vertex::VERTEX_SIZE;

	if (isDynamic) {
		GLCALL(glBufferData(GL_ARRAY_BUFFER, size, Vertex::buildData(vertices), GL_DYNAMIC_DRAW));
	}
	else {
		GLCALL(glBufferData(GL_ARRAY_BUFFER, size, Vertex::buildData(vertices), GL_STATIC_DRAW));
	}
}

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
	: m_IsDynamic(false) {

	GLCALL(glGenBuffers(1, &m_RendererID));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
	GLCALL(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const {
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const {
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::UpdateBufferData(std::vector<Vertex*> vertices) {

	Bind();
	unsigned int size = vertices.size() * Vertex::VERTEX_SIZE;
	GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size, Vertex::buildData(vertices)))

}