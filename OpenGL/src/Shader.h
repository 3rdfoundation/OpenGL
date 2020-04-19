#pragma once

class Shader {

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	std::unordered_map<std::string,int> m_UniformLocationCache;

	struct ShaderProgramSource {
		std::string VertexSource;
		std::string FragmentSource;
	};

	int GetUniformLocation(const std::string& name);

	ShaderProgramSource LoadShaders(const std::string filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

};