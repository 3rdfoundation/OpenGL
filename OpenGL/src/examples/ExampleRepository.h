#pragma once
#include "Example.h"

namespace example {

	class ExampleRepository {

	private:
		std::vector<Example*> m_Examples;
	
	public:
		ExampleRepository();
		~ExampleRepository() {};
		std::vector<Example*> GetExamples();

	};

}