#pragma once
#include "Example.h"
#include "Texture.h"

namespace example {

	class ExampleBatchDynamicGeometry : public Example {

	public:
		ExampleBatchDynamicGeometry();
		~ExampleBatchDynamicGeometry();

		void Setup() override;
		void Teardown() override;

		std::string GetDescription() override;
		void OnUpdate(float DeltaTime) override;
		void OnRender(Renderer& renderer) override;
		void OnImGuiRender() override;

		// CUSTOM STUFF
		float m_ClearColor[4];
		unsigned int m_Indices[6 * 2];

		// Replacement for m_Positions
		std::vector<Vertex*> m_Vertices;
		
		glm::vec3* m_Translation1;

		std::unique_ptr<VertexArray> m_VertexArray;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

		glm::mat4 m_Projection;
		glm::mat4 m_View;
		std::unique_ptr<Shader> m_Shader;

		std::unique_ptr<Texture> m_Texture_1;
		std::unique_ptr<Texture> m_Texture_2;

		void AdjustVertices(float speed);
		float m_CycleDirection;
		float m_XPosition;

	};

}