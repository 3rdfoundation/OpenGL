#include "opengl.h"
#include "ExampleRepository.h"
#include "ExampleBatching.h"
#include "ExampleClearColor.h"
#include "ExampleCyclingColor.h"
#include "ExampleDualRender.h"
#include "ExampleVertexColor.h"

namespace example {

	ExampleRepository::ExampleRepository() {
		m_Examples.push_back(new ExampleBatching());
		m_Examples.push_back(new ExampleClearColor());
		m_Examples.push_back(new ExampleCyclingColor());
		m_Examples.push_back(new ExampleDualRender());
		m_Examples.push_back(new ExampleVertexColor());
	}

	std::vector<Example*> ExampleRepository::GetExamples() {
		return m_Examples;
	}

}