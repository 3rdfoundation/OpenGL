#pragma once
#include "Example.h"

namespace example {

	class ExampleClearColor : public Example {
	
	public:
		ExampleClearColor();
		~ExampleClearColor();

		void Setup() override;
		void Teardown() override;

		std::string GetDescription() override;
		void OnUpdate(float DeltaTime) override;
		void OnRender(Renderer& renderer) override;
		void OnImGuiRender() override;

	private:
		float m_ClearColor[4];
	};

}