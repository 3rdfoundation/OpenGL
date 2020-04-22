#pragma once

namespace test {

	class Test {
	
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float DeltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}

	};

}