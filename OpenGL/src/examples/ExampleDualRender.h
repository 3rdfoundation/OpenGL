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

		void OnUpdate(float DeltaTime) override;
		void OnRender(Renderer& renderer) override;
		void OnImGuiRender() override;

	//private:
		float m_ClearColor[4];
		float m_Positions[16];
		unsigned int m_Indices[6];
		
		glm::vec3* m_Translation1;
		glm::vec3* m_Translation2;

		VertexArray* m_VertexArray;
		VertexBuffer* m_VertexBuffer;
		IndexBuffer* m_IndexBuffer;

		glm::mat4 m_Projection;
		glm::mat4 m_View;
		Shader* m_Shader;
		Texture* m_Texture;

	};

}