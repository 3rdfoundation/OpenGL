#include "opengl.h"

#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

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
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

	float positions[] = {
		-4.0f, -4.0f, 0.0f, 0.0f, // 0 : X, Y, U, V (pos = X,Y) (tex coord = U,V)
		 4.0f, -4.0f, 1.0f, 0.0f, // 1
		 4.0f,  4.0f, 1.0f, 1.0f, // 2
		-4.0f,  4.0f, 0.0f, 1.0f  // 3
	};

	// ----------------------------------------------------------------------------
	// Create an index buffer
	// ----------------------------------------------------------------------------

	unsigned int indices[]{
		0, 1, 2, // TRIANGLE 1
		2, 3, 0  // TRIANGLE 2
	};

	// Set up blending for an alpha channel
	GLCALL(glEnable(GL_BLEND));
	GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA));

	// ----------------------------------------------------------------------------
	// Setup everything for OpenGL
	// ----------------------------------------------------------------------------

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
	// > this represents the ratio of our window size (640 x 480)
	// > 6/4.5 = 4:3 ratio
	// > things further away do not get smaller (like ortho in blender)
	// > This is the actual size of our window in vertex terms
	glm::mat4 projection = glm::ortho(-6.f, 6.f, -4.5f, 4.5f, -1.0f, 1.0f);

	// NOTE: glm::mat4(1.0f) = identity matrix (matrix equivalent of 1)
	// > this creates an X offset of -1.f (moves to left ... simulates camera moving to the right)
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-1.f, 0, 0));
	
	// > this creates a Y offset of .25f (move up)
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, .25f, 0));

	// Create the MVP matrix (CPU side calc since it is not in the shader)
	glm::mat4 mvp = projection * view * model;

	// Load the shaders
	Shader shader("resources/shaders/basic.shader");
	shader.Bind();
	shader.SetUniformMat4f("u_MVP", mvp);

	Texture texture("resources/textures/half_life_alyx.png");
	texture.Bind(); // 0 = texture slot
	shader.SetUniform1i("u_Texture", 0); // make texture slot (0) available to shader code

	Renderer renderer;

	// ----------------------------------------------------------------------------
	// Loop until the user closes the window
	// ----------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window)) {

		renderer.Clear();
		renderer.Draw(va, ib, shader);

		// Swap front and back buffers
		GLCALL(glfwSwapBuffers(window));

		// Poll for and process events
		GLCALL(glfwPollEvents());
	}

	// END SCOPE TO ENSURE TERMINATE DOES NOT HANG
	// > covered at end of "Abstracting OpenGL into Classes"
	}

	glfwTerminate();
	return 0;

}