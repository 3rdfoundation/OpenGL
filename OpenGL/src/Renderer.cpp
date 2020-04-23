#include "opengl.h"
#include "Renderer.h"

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
	shader.Bind();
	//shader.SetUniform4f("u_Color", red, 0.5f, 1.0f, 1.0f);
	va.Bind();
	ib.Bind();

	//std::cout << "RENDERER: index buffer count: " << ib.GetCount() << std::endl;
	GLCALL(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const {
	GLCALL(glClear(GL_COLOR_BUFFER_BIT));
}