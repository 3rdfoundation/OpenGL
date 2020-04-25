#pragma once
#include "Example.h"
#include "Texture.h"

namespace example {

	class ExampleCyclingColor : public Example {

	public:
		ExampleCyclingColor();
		~ExampleCyclingColor();

		void Setup() override;
		void Teardown() override;

		void OnUpdate(float DeltaTime) override;
		void OnRender(Renderer& renderer) override;
		void OnImGuiRender() override;

		// CUSTOM STUFF
		float m_ClearColor[4];
		float m_Positions[16];
		unsigned int m_Indices[6];
		
		glm::vec3* m_Translation1;

		std::unique_ptr<VertexArray> m_VertexArray;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

		glm::mat4 m_Projection;
		glm::mat4 m_View;
		std::unique_ptr<Shader> m_Shader;

		float m_CycleDirection;
		float m_Red;

	};

}