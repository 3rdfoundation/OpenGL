#include "opengl.h"

#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "imgui_impl_glfw_gl3.h"

#include "examples\Example.h"
#include "examples\ExampleDualRender.h"
#include "examples\ExampleRepository.h"

// ============================================================================
// SUPPORTING METHODS
// ============================================================================

// GLFW : Set up the windows to draw onto
GLFWwindow* CreateWindow() {

	// Initialize the library
	if (!glfwInit()) {
		return nullptr;
	}

	// Force profile to "Core Profile" on OpenGL v3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a windowed mode window and its OpenGL context
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return nullptr;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	// Control the framerate
	// > sync with monitor framerate
	glfwSwapInterval(1);

	// ImGUI setup
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();

	return window;

}

// GLEW : INITIALIZE
int InitializeOpenGLGraphicCardInterface() {

	// Glew init must occur after having a valid window context
	if (glewInit() != GLEW_OK) {
	std::cout << "GlewInit failed" << std::endl;
	std::cin.get();
	return -1;
	}

	// 4.6.0 NVIDIA 432
	// 4.6.0 = OpenGL version
	std::cout << glGetString(GL_VERSION) << std::endl;

	return 0;

}

int main() {

	// GLFW & IMGUI
	GLFWwindow* window = CreateWindow();
	if (window == nullptr) {
		return -1;
	}

	// GLEW
	if (InitializeOpenGLGraphicCardInterface() != 0) {
		return -1;
	}

	// SCOPE TO ENSURE TERMINATE DOES NOT HANG
	{

	// Get the list of examples
	example::ExampleRepository exampleRepo;
	std::vector<example::Example*> examples = exampleRepo.GetExamples();

	// Build boolean array to track which menu item is clicked
	int currentExample = -1;
	const int TOTAL_EXAMPLES_POSSIBLE = 100;
	bool selectedExampleMenuItem[TOTAL_EXAMPLES_POSSIBLE];
	for (int i = 0; i < examples.size(); i++) {
		selectedExampleMenuItem[i] = false;
	}

	Renderer renderer;
	while (!glfwWindowShouldClose(window)) {

		renderer.Clear();

		if (currentExample != -1) {
			examples[currentExample]->OnUpdate(0.0f);
			examples[currentExample]->OnRender(renderer);
		}

		// ImGUI frame
		ImGui_ImplGlfwGL3_NewFrame();

		// ImGUI Demo Window
		bool show_demo_window = true;
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
		//ImGui::ShowDemoWindow(&show_demo_window);

		bool* p_open = NULL;
		if (!ImGui::Begin("OpenGL Playland", p_open, ImGuiWindowFlags_MenuBar)) {
			ImGui::End();
			return 1;
		}
		ImGui::PushItemWidth(-140);

		// Menu : This just flips a boolean when clicked
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("OpenGL Examples")) {
				using Iter = std::vector<example::Example>::const_iterator;
				for (int i = 0; i < examples.size(); i++) {
					ImGui::MenuItem(examples[i]->GetDescription().c_str(), NULL, &selectedExampleMenuItem[i]);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// Note: need to use examples for size since it has the actual entries
		for (int i = 0; i < examples.size(); i++) {
			// Teardown : This happens 2 ways:
			// 1 : If an example is active an you click on its menu item
			// 2 : If an example is active and you click on a different menu item
			if (currentExample != -1 
				&& (i == currentExample && !selectedExampleMenuItem[i] 
					|| i != currentExample && selectedExampleMenuItem[i])) {
				// tear down prior example
				examples[currentExample]->Teardown();
				selectedExampleMenuItem[currentExample] = false;
				currentExample = -1;
			}
			// Setup
			if (selectedExampleMenuItem[i] && currentExample == -1) {
				// setup current example
				examples[i]->Setup();
				currentExample = i;
			}
		}

		if (currentExample != -1) {
			ImGui::Text("ACTIVE EXAMPLE: %s", examples[currentExample]->GetDescription());
			ImGui::Separator();
			examples[currentExample]->OnImGuiRender();
		}

		ImGui::End();

		// ImGUI render
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap front and back buffers
		GLCALL(glfwSwapBuffers(window));

		// Poll for and process events
		GLCALL(glfwPollEvents());
	}

	// END SCOPE TO ENSURE TERMINATE DOES NOT HANG
	// > covered at end of "Abstracting OpenGL into Classes"
	}

	// ImGUI clean up
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;

}