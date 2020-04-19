#include "opengl.h"
#include "Shader.h"
#include "OpenGLUtils.h"

Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererID(0) {

	// NOTE: Path is relative to project directory
	Shader::ShaderProgramSource shaders = LoadShaders(filepath);
	m_RendererID = CreateShader(shaders.VertexSource, shaders.FragmentSource);
}

Shader::~Shader() {
	GLCALL(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const {
	GLCALL(glUseProgram(m_RendererID));
}

void Shader::Unbind() const {
	GLCALL(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
	int location = GetUniformLocation(name);
	GLCALL(glUniform4f(location, v0, v1, v2, v3));
}

// ============================================================================
// PRIVATE METHODS
// ============================================================================

int Shader::GetUniformLocation(const std::string& name) {
	int location = -1;
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		location = m_UniformLocationCache[name];
	}
	else {
		GLCALL(location = glGetUniformLocation(m_RendererID, "u_Color"));
		if (location == -1) {
			std::cout << "WARNING: missing uniform location: " << name << std::endl;
		}
		m_UniformLocationCache[name] = location;
	}
	return location;
}

// Load the shaders out of a file
Shader::ShaderProgramSource Shader::LoadShaders(const std::string filePath) {
	std::stringstream shaders[2];

	enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

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

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
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
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

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