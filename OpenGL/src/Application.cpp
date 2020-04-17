#include "opengl.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() {
	// while body is irrelevant... we just want to call until we get GL_NO_ERROR
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "GL Error: " << error << 
			" : " << function << " : " << file << " : " << line << std::endl;
		return false;
	}
	return true;
}

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

	float positions[] = {
		-0.5f, -0.5f, // 0
		 0.5f, -0.5f, // 1
		 0.5f,  0.5f, // 2
		-0.5f,  0.5f  // 3
	};

	unsigned int indices[]{
		0, 1, 2, // TRIANGLE 1
		2, 3, 0  // TRIANGLE 2
	};

	// Vertex Buffer
	unsigned int buffer;
	GLCALL(glGenBuffers(1, &buffer));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

	GLCALL(glEnableVertexAttribArray(0));
	GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

	// Index Buffer Object
	unsigned int ibo;
	GLCALL(glGenBuffers(1, &ibo));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

	// Create shader source code
	
	// layout(location = 0) : 0 is from the glVertexAttribPointer
	// vec4 being used for a 2 entry attribute. GL will auto convert it into a 4 entry vector.

	// Path is relative to project directory
	ShaderProgramSource shaders = LoadShaders("resources/shaders/basic.shader");

	// compile the shader
	unsigned int shader = CreateShader(shaders.VertexSource, shaders.FragmentSource);
	GLCALL(glUseProgram(shader));

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window)) {
		
		// Render here
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));

		// Use index buffer (drawing 6 indices)
		GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		// Swap front and back buffers
		GLCALL(glfwSwapBuffers(window));

		// Poll for and process events
		GLCALL(glfwPollEvents());
	}

	GLCALL(glDeleteProgram(shader));
	GLCALL(glfwTerminate());
	return 0;

}