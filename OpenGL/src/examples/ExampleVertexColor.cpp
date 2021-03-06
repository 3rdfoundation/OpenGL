#include "opengl.h"
#include "ExampleVertexColor.h"

#include "opengl.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "imgui_impl_glfw_gl3.h"

namespace example {

	// Positions =  X, Y, U, V, C (pos = X,Y) (tex coord = U,V) (channel = C)
	// Indexes = 2 objects each with 2 triangles (3 vertices per triangle)
	ExampleVertexColor::ExampleVertexColor() :

		m_ClearColor { 
			0.8f, 0.3f, 0.2f, 1.0f },

		m_Positions {
			-150.0f, -150.0f, 0.0f, 0.0f, 0.5f, 0.3f, 0.4f, 1.f,
			 150.0f, -150.0f, 1.0f, 0.0f, 0.5f, 0.3f, 0.4f, 1.f,
			 150.0f,  150.0f, 1.0f, 1.0f, 0.5f, 0.3f, 0.4f, 1.f,
			-150.0f,  150.0f, 0.0f, 1.0f, 0.5f, 0.3f, 0.4f, 1.f,

			-150.0f,  200.0f, 0.0f, 0.0f, 0.2f, 0.5f, 0.4f, 1.f,
			150.0f,   200.0f, 1.0f, 0.0f, 0.2f, 0.5f, 0.4f, 1.f,
			150.0f,   500.0f, 1.0f, 1.0f, 0.2f, 0.5f, 0.4f, 1.f,
			-150.0f,  500.0f, 0.0f, 1.0f, 0.2f, 0.5f, 0.4f, 1.f },

		m_Indices {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4 } {

		m_Translation1 = new glm::vec3(200.f, 200.f, 0);

	}

	ExampleVertexColor::~ExampleVertexColor() {
	}

	std::string ExampleVertexColor::GetDescription() {
		return "Vertex Color";
	}

	void ExampleVertexColor::Setup() {

		// Vertex Array
		m_VertexArray = std::make_unique<VertexArray>();

		// Vertex Buffer
		m_VertexBuffer = std::make_unique<VertexBuffer>(m_Positions, 8 * 8 * sizeof(float));

		// Vertex Buffer Layout of a single row (X,Y + U,V + C)
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		layout.Push<float>(4);
		m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(m_Indices, 4 * 3);

		// Create a 4:3 orthographic projection matrix
		// > this represents the ratio of our window size (1024 x 760)
		// > to keep things simple we are using the same units as window pixels
		// > things further away do not get smaller (like ortho in blender)
		m_Projection = glm::ortho(0.f, 1024.f, 0.f, 768.f, -1.0f, 1.0f);

		// NOTE: glm::mat4(1.0f) = identity matrix (matrix equivalent of 1)
		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0));

		// Load the shaders
		m_Shader = std::make_unique<Shader>("resources/shaders/texture_vertex_color.shader");
		m_Shader->Bind();
	}

	void ExampleVertexColor::Teardown() {
		GLCALL(glClearColor(0, 0, 0, 0));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));
		GLCALL(m_Shader->Unbind());
	}

	void ExampleVertexColor::OnUpdate(float DeltaTime) {
	}

	void ExampleVertexColor::OnRender(Renderer& renderer) {

		// Set a non-black background so I can see wtf is going on
		GLCALL(glClearColor(.2f, .3f, .4f, 0));

		glm::mat4 model = glm::translate(glm::mat4(1.0f), *m_Translation1);
		glm::mat4 mvp = m_Projection * m_View * model;

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp);
		renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);

	}

	void ExampleVertexColor::OnImGuiRender() {

		// passing in memory address of first entry of translation (&translation .x)
		ImGui::SliderFloat3("Translation 1", &m_Translation1->x, 0.0f, 1024.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	}

}