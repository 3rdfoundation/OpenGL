#include "opengl.h"
#include "ExampleClearColor.h"

namespace example {

	ExampleClearColor::ExampleClearColor()
		: m_ClearColor{ 0.2f, 0.3f, 0.8f, 1.0f } {
	}

	ExampleClearColor::~ExampleClearColor() {
	}

	std::string ExampleClearColor::GetDescription() {
		return "ClearColor";
	}

	void ExampleClearColor::Setup() {
	}

	void ExampleClearColor::Teardown() {
		GLCALL(glClearColor(0, 0, 0, 0));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));
	}

	void ExampleClearColor::OnUpdate(float DeltaTime) {
	}

	void ExampleClearColor::OnRender(Renderer& renderer) {
		GLCALL(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));
	}

	void ExampleClearColor::OnImGuiRender() {
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
	}

}