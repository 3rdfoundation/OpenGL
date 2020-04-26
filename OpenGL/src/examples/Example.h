#pragma once

#include "Renderer.h"

namespace example {

	class Example {
	
	public:

		Example() {}
		virtual ~Example() {}

		virtual std::string GetDescription() = 0;
		virtual void Setup() {}
		virtual void Teardown() {}

		virtual void OnUpdate(float DeltaTime) {}
		virtual void OnRender(Renderer& renderer) {}
		virtual void OnImGuiRender() {
			std::cout << "Example.OnImGuiRender()" << std::endl;
		}

	};

}