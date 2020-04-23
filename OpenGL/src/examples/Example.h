#pragma once

#include "Renderer.h"

namespace example {

	class Example {
	
	public:
		std::string m_Description;

		Example() {}
		virtual ~Example() {}

		std::string GetDescription() { return m_Description; }
		virtual void Setup() {}
		virtual void Teardown() {}

		virtual void OnUpdate(float DeltaTime) {}
		virtual void OnRender(Renderer& renderer) {}
		virtual void OnImGuiRender() {
			std::cout << "Example.OnImGuiRender()" << std::endl;
		}

	};

}