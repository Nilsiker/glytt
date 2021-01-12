#pragma once
#include <iostream>

class Shader {
private:
	unsigned int m_RendererID;
	// TODO add map to cache uniform locations
public:
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	~Shader();

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v4);

	void Bind() const;
	void Unbind() const;
private:
	unsigned int CompileShader(unsigned int type, const std::string& filepath);
	int GetUniformLocation(const std::string& name);
};