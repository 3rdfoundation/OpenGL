#include "opengl.h"
#include "VertexArray.h"

VertexArray::VertexArray() {
	GLCALL(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray() {
	GLCALL(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		GLCALL(glEnableVertexAttribArray(i));
		// NOTE: this "secretly" links the currently bound vertex buffer to the vertex array object
		GLCALL(glVertexAttribPointer(
			i, element.count, element.type, element.normalized, layout.GetStride(), (const void*) offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const {
	// Getting GL_INVALID_OPERATION
	GLCALL(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const {
	GLCALL(glBindVertexArray(0));
}