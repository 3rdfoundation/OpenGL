#include "opengl.h"
#include "ExampleRepository.h"
#include "ExampleClearColor.h"
#include "ExampleCyclingColor.h"
#include "ExampleDualRender.h"

namespace example {

	ExampleRepository::ExampleRepository() {
		m_Examples.push_back(new ExampleClearColor());
		m_Examples.push_back(new ExampleCyclingColor());
		m_Examples.push_back(new ExampleDualRender());
	}

	std::vector<Example*> ExampleRepository::GetExamples() {
		return m_Examples;
	}

}