#include "opengl.h"
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

// ----------------------------------------------------------------------------
// SHADER FILE RELATED
// ----------------------------------------------------------------------------

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

// Load the shaders out of a file
static ShaderProgramSource LoadShaders(const std::string filePath) {
	std::stringstream shaders[2];

	enum class ShaderType {NONE=-1, VERTEX=0, FRAGMENT=1};

	std::ifstream stream(filePath);
	std::string line;
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		// Skip comments
		if (line.find("# ----") == 0) {
			continue;
		}
		// New shader found
		else if (line.find("# SHADER ") == 0) {
			if (line.find(" VERTEX") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find(" FRAGMENT") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			shaders[(int)type] << line << "\n";
		}
	}

	return { shaders[0].str(), shaders[1].str() };
}

// ----------------------------------------------------------------------------
// SHADER COMPILATION RELATED
// ----------------------------------------------------------------------------

static unsigned int CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// Error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*) alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

// Provide OpenGL our shader source code and compile it
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	// delete the intermediate shaders (think of them like C++ intermediate files)
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;

}

// ----------------------------------------------------------------------------
// CUSTOM CODE
// ----------------------------------------------------------------------------

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
		-0.5f, -0.5f, // 0
		 0.5f, -0.5f, // 1
		 0.5f,  0.5f, // 2
		-0.5f,  0.5f  // 3
	};

	// ----------------------------------------------------------------------------
	// Create an index buffer
	// ----------------------------------------------------------------------------

	unsigned int indices[]{
		0, 1, 2, // TRIANGLE 1
		2, 3, 0  // TRIANGLE 2
	};

	// Create the vertex array object
	unsigned int vao;
	GLCALL(glGenVertexArrays(1, &vao));
	GLCALL(glBindVertexArray(vao));

	// Vertex Array
	VertexArray va;

	// Vertex Buffer
	VertexBuffer vb(positions, 4 * 2 * sizeof(float));

	// Vertex Buffer Layout #1 (2 dimensional position)
	VertexBufferLayout layout;
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	// Index Buffer Object
	IndexBuffer ib(indices, 2 * 3);

	// Path is relative to project directory
	ShaderProgramSource shaders = LoadShaders("resources/shaders/basic.shader");

	// compile the shader
	unsigned int shader = CreateShader(shaders.VertexSource, shaders.FragmentSource);

	// Use the shader
	GLCALL(glUseProgram(shader));

	// Get u_Color memory location
	GLCALL(int location = glGetUniformLocation(shader, "u_Color"));
	ASSERT(location != -1);

	// ----------------------------------------------------------------------------
	// UNBIND EXAMPLE (using 0 buffer ubinds our buffer)
	// ----------------------------------------------------------------------------

	GLCALL(glBindVertexArray(0));
	GLCALL(glUseProgram(0));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	// ----------------------------------------------------------------------------
	// Loop until the user closes the window
	// ----------------------------------------------------------------------------
	float red = 0.0f;
	while (!glfwWindowShouldClose(window)) {
		
		// Render here
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));

		// Use index buffer (drawing 6 indices)
		red += .05f;
		if (red > 1) {
			red = 0;
		}

		// ----------------------------------------------------------------------------
		// BIND SO THAT DRAW WORKS
		// ----------------------------------------------------------------------------

		GLCALL(glUseProgram(shader));
		GLCALL(glUniform4f(location, red, 0.5f, 1.0f, 1.0f));

		va.Bind();
		ib.Bind();

		// ----------------------------------------------------------------------------
		// DRAW & DISPLAY
		// ----------------------------------------------------------------------------
		GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		// Swap front and back buffers
		GLCALL(glfwSwapBuffers(window));

		// Poll for and process events
		GLCALL(glfwPollEvents());
	}

	GLCALL(glDeleteProgram(shader));

	// END SCOPE TO ENSURE TERMINATE DOES NOT HANG
	// > covered at end of "Abstracting OpenGL into Classes"
	}

	glfwTerminate();
	return 0;

}