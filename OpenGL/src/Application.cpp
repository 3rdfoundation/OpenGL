#include "opengl.h"

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

int main() {

	//std::cout << "hello world" << std::endl;
	//std::cin.get();

	GLFWwindow* window;

	// Initialize the library
	if (!glfwInit()) {
		return -1;
	}

	// Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	// Glew init must occur after having a valid window context
	if (glewInit() != GLEW_OK) {
		std::cout << "GlewInit failed" << std::endl;
		std::cin.get();
		return -1;
	}

	// 4.6.0 NVIDIA 432
	// 4.6.0 = OpenGL version
	std::cout << glGetString(GL_VERSION) << std::endl;

	// ---------------------------------------------------------------------------
	// Create a modern OpenGL buffer
	// ---------------------------------------------------------------------------

	float positions[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};

	// this creates a buffer and gives us back the id of the buffer
	unsigned int bufferID;
	glGenBuffers(1, &bufferID);

	// Select the buffer by ID
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);

	// We have 3 vertexes, each only a position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	// Fill the buffer with data
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	// Create shader source code
	
	// layout(location = 0) : 0 is from the glVertexAttribPointer
	// vec4 being used for a 2 entry attribute. GL will auto convert it into a 4 entry vector.
	std::string vertexShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec4 position;\n"
		"\n"
		"void main() {\n"
		"  gl_Position = position;\n"
		"}\n";

	// color = vec4(R,G,B,Alpha) : values are 0 thru 1
	std::string fragmentShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"\n"
		"void main() {\n"
		"  color = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}\n";

	// compile the shader
	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window)) {
		
		// Render here
		glClear(GL_COLOR_BUFFER_BIT);

		// Use buffer
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	glDeleteProgram(shader);

	glfwTerminate();
	return 0;

}