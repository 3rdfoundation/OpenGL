#include "opengl.h"
#include "ExampleBatchDynamicGeometry.h"

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
	ExampleBatchDynamicGeometry::ExampleBatchDynamicGeometry() :

		m_ClearColor { 
			0.8f, 0.3f, 0.2f, 1.0f },

		m_Indices {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4 } {

		m_Translation1 = new glm::vec3(200.f, 200.f, 0);

		m_CycleDirection = 1.f;
		m_XPosition = -150.f;

	}

	ExampleBatchDynamicGeometry::~ExampleBatchDynamicGeometry() {
	}

	std::string ExampleBatchDynamicGeometry::GetDescription() {
		return "Dynamic Mesh";
	}

	void ExampleBatchDynamicGeometry::Setup() {

		// Vertex Array
		m_VertexArray = std::make_unique<VertexArray>();

		// Vertex Buffer
		AdjustVertices(0);
		m_VertexBuffer = std::make_unique<VertexBuffer>(m_Vertices, true, 100);

		// Vertex Buffer Layout of a single row (X,Y + U,V + C)
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(4);
		layout.Push<float>(2);
		layout.Push<float>(1);
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
		m_Shader = std::make_unique<Shader>("resources/shaders/dynamic_mesh.shader");
		m_Shader->Bind();

		m_Texture_1 = std::make_unique<Texture>("resources/textures/half_life_alyx.png");
		m_Texture_1->Bind(0);

		m_Texture_2 = std::make_unique<Texture>("resources/textures/half_life_alyx_2.png");
		m_Texture_2->Bind(1);

		int channels[2] = { 0, 1 };
		m_Shader->SetUniform1iv("u_Texture", 2, channels);
	}

	void ExampleBatchDynamicGeometry::Teardown() {
		GLCALL(glClearColor(0, 0, 0, 0));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));
		GLCALL(m_Texture_1->Unbind());
		GLCALL(m_Texture_2->Unbind());
		GLCALL(m_Shader->Unbind());
	}

	void ExampleBatchDynamicGeometry::OnUpdate(float DeltaTime) {
	}

	void ExampleBatchDynamicGeometry::OnRender(Renderer& renderer) {

		// Set a non-black background so I can see wtf is going on
		GLCALL(glClearColor(.2f, .3f, .4f, 0));

		glm::mat4 model = glm::translate(glm::mat4(1.0f), *m_Translation1);
		glm::mat4 mvp = m_Projection * m_View * model;

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp);
		
		GLCALL(glBindTextureUnit(0, m_Texture_1->m_RendererID));
		GLCALL(glBindTextureUnit(1, m_Texture_2->m_RendererID));
		
		AdjustVertices(4.f);
		m_VertexBuffer->UpdateBufferData(m_Vertices);

		renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);

	}

	void ExampleBatchDynamicGeometry::OnImGuiRender() {

		// passing in memory address of first entry of translation (&translation .x)
		ImGui::SliderFloat3("Translation 1", &m_Translation1->x, 0.0f, 1024.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	}

	void ExampleBatchDynamicGeometry::AdjustVertices(float speed) {

		// Adjust a vertex position every frame
		m_XPosition += speed * m_CycleDirection;
		if (m_XPosition >= 150.f) {
			m_CycleDirection = -1;
			m_XPosition = 150.f;
		}
		else if (m_XPosition < -150.f) {
			m_CycleDirection = 1;
			m_XPosition = -150.f;
		}

		// clear out the vertices
		m_Vertices.clear();

		// Mesh 1
		m_Vertices.push_back(new Vertex({ m_XPosition, -150.f, 0.f }, { 1.f, 0.f, 0.f, 1.f }, { 0.f, 0.f }, 0.f));
		m_Vertices.push_back(new Vertex({ 150.f, -150.f, 0.f }, { 1.f, 0.f, 0.f, 1.f }, { 1.f, 0.f }, 0.f));
		m_Vertices.push_back(new Vertex({ 150.f,  150.f, 0.f }, { 1.f, 0.f, 0.f, 1.f }, { 1.f, 1.f }, 0.f));
		m_Vertices.push_back(new Vertex({ m_XPosition,  150.f, 0.f }, { 1.f, 0.f, 0.f, 1.f }, { 0.f, 1.f }, 0.f));

		// Mesh 2
		m_Vertices.push_back(new Vertex({ m_XPosition,  200.f, 0.f }, { 1.f, 0.f, 0.f, 1.f }, { 0.f, 0.f }, 1.f));
		m_Vertices.push_back(new Vertex({ 150.f,  200.f, 0.f }, { 1.f, 0.f, 0.f, 1.f }, { 1.f, 0.f }, 1.f));
		m_Vertices.push_back(new Vertex({ 150.f,  500.f, 0.f }, { 1.f, 0.f, 0.f, 1.f }, { 1.f, 1.f }, 1.f));
		m_Vertices.push_back(new Vertex({ m_XPosition,  500.f, 0.f }, { 1.f, 0.f, 0.f, 1.f }, { 0.f, 1.f }, 1.f));

	}

}