#include "opengl.h"

#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "imgui_impl_glfw_gl3.h"

#include "tests\TestClearColor.h"

int main() {

	// ----------------------------------------------------------------------------
	// Set up the windows to draw onto
	// ----------------------------------------------------------------------------

	//std::cout << "hello world" << std::endl;
	//std::cin.get();

	GLFWwindow* window;

	// Initialize the library
	if (!glfwInit()) {
		return -1;
	}

	// Force profile to "Core Profile" on OpenGL v3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(1024, 768, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	// Control the framerate
	// > sync with monitor framerate
	glfwSwapInterval(1);

	// ----------------------------------------------------------------------------
	// INITIALIZE GLEW
	// ----------------------------------------------------------------------------

	// Glew init must occur after having a valid window context
	if (glewInit() != GLEW_OK) {
		std::cout << "GlewInit failed" << std::endl;
		std::cin.get();
		return -1;
	}

	// 4.6.0 NVIDIA 432
	// 4.6.0 = OpenGL version
	std::cout << glGetString(GL_VERSION) << std::endl;

	// SCOPE TO ENSURE TERMINATE DOES NOT HANG
	{

	// ---------------------------------------------------------------------------
	// Create a modern OpenGL buffer
	// ---------------------------------------------------------------------------

		/*
	// Origin (center) is 0,0
	float positions[] = {
		-150.0f, -150.0f, 0.0f, 0.0f, // 0 : X, Y, U, V (pos = X,Y) (tex coord = U,V)
		 150.0f, -150.0f, 1.0f, 0.0f, // 1
		 150.0f,  150.0f, 1.0f, 1.0f, // 2
		-150.0f,  150.0f, 0.0f, 1.0f  // 3
	};

	// ----------------------------------------------------------------------------
	// Create an index buffer
	// ----------------------------------------------------------------------------

	unsigned int indices[]{
		0, 1, 2, // TRIANGLE 1
		2, 3, 0  // TRIANGLE 2
	};
	*/

	// Set up blending for an alpha channel
	GLCALL(glEnable(GL_BLEND));
	GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA));

	// ----------------------------------------------------------------------------
	// Setup everything for OpenGL
	// ----------------------------------------------------------------------------

	/*
	// Vertex Array
	VertexArray va;

	// Vertex Buffer
	VertexBuffer vb(positions, 4 * 4 * sizeof(float));

	// Vertex Buffer Layout #1 (2 dimensional position)
	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	// Index Buffer Object
	IndexBuffer ib(indices, 2 * 3);

	// Create a 4:3 orthographic projection matrix
	// > this represents the ratio of our window size (1024 x 760)
	// > to keep things simple we are using the same units as window pixels
	// > things further away do not get smaller (like ortho in blender)
	glm::mat4 projection = glm::ortho(0.f, 1024.f, 0.f, 768.f, -1.0f, 1.0f);

	// NOTE: glm::mat4(1.0f) = identity matrix (matrix equivalent of 1)
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0));

	// Load the shaders
	Shader shader("resources/shaders/basic.shader");
	shader.Bind();

	Texture texture("resources/textures/half_life_alyx.png");
	texture.Bind(); // 0 = texture slot
	shader.SetUniform1i("u_Texture", 0); // make texture slot (0) available to shader code
	*/

	Renderer renderer;

	// ImGUI setup
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();

	/*
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	
	glm::vec3 translation1(200.f, 200.f, 0);
	glm::vec3 translation2(200.f, 550.f, 0);
	*/

	test::TestClearColor testClearColor;

	// TODO: initialize a list of test classes with menu descriptions
	int currentTest = -1;
	bool selected_test[4] = { false, false, false, false };

	// ----------------------------------------------------------------------------
	// Loop until the user closes the window
	// ----------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window)) {

		renderer.Clear();

		testClearColor.OnUpdate(0.0f);
		testClearColor.OnRender();

		// ImGUI frame
		ImGui_ImplGlfwGL3_NewFrame();

		// ImGUI Demo Window
		bool show_demo_window = true;
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
		//ImGui::ShowDemoWindow(&show_demo_window);

		// ==============

		bool* p_open = NULL;
		if (!ImGui::Begin("ImGui Demo", p_open, ImGuiWindowFlags_MenuBar)) {
			// Early out if the window is collapsed, as an optimization.
			ImGui::End();
			return 1;
		}
		ImGui::PushItemWidth(-140);
		ImGui::Text("dear imgui says hello. (%s)", IMGUI_VERSION);

		// Menu : This just flips a boolean
		// TODO: build this with a loop instead where menu description and class name are already known
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("Tests")) {
				ImGui::MenuItem("Clear Color Test", NULL, &selected_test[0]);
				ImGui::MenuItem("Color Rectangle", NULL, &selected_test[1]);
				ImGui::MenuItem("Texture Rectangle", NULL, &selected_test[2]);
				ImGui::MenuItem("1 Buffer, 2 MVPs", NULL, &selected_test[3]);
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::End();

		// TODO: code this
		//
		// First, we have to have all test classes pre-constructed because C++ does not have reflection
		// > This means the constructor/destructor are not the place for setup/teardown (easy enough)
		//
		// See if a menu item was clicked. If so:
		// 1. teardown prior test (currentTest) if it exists
		// 2. setup next test
		// 3. set currentTest = selected test
		// 4. reset the booleans to false

		// ==============

		/*
		glm::mat4 model = glm::translate(glm::mat4(1.0f), translation1);
		glm::mat4 mvp = projection * view * model;

		shader.Bind();
		shader.SetUniformMat4f("u_MVP", mvp);
		renderer.Draw(va, ib, shader);

		glm::mat4 model2 = glm::translate(glm::mat4(1.0f), translation2);
		glm::mat4 mvp2 = projection * view * model2;

		shader.Bind();
		shader.SetUniformMat4f("u_MVP", mvp2);
		renderer.Draw(va, ib, shader);

		// ImGUI example
		{
			// passing in memory address of first entry of translation (&translation .x)
			ImGui::SliderFloat3("Translation 1", &translation1.x, 0.0f, 1024.0f);   
			ImGui::SliderFloat3("Translation 2", &translation2.x, 0.0f, 1024.0f);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
		*/

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