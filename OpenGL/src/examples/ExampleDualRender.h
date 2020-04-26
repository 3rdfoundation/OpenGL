#pragma once
#include "Example.h"
#include "Texture.h"

namespace example {

	class ExampleDualRender : public Example {

	public:
		ExampleDualRender();
		~ExampleDualRender();

		void Setup() override;
		void Teardown() override;

		std::string GetDescription() override;
		void OnUpdate(float DeltaTime) override;
		void OnRender(Renderer& renderer) override;
		void OnImGuiRender() override;

		// CUSTOM STUFF
		float m_ClearColor[4];
		float m_Positions[16];
		unsigned int m_Indices[6];
		
		glm::vec3* m_Translation1;
		glm::vec3* m_Translation2;

		std::unique_ptr<VertexArray> m_VertexArray;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

		glm::mat4 m_Projection;
		glm::mat4 m_View;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

	};

}