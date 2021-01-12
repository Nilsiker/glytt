#include "Shader.h"

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <vector>

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    : m_RendererID(0)
{
    m_RendererID = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShaderPath);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderPath);

    glAttachShader(m_RendererID, vs);
    glAttachShader(m_RendererID, fs);
    glLinkProgram(m_RendererID);
    glValidateProgram(m_RendererID);

    GLint isLinked = 0;
    glGetProgramiv(m_RendererID, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

        // The program is useless now. So delete it.
        glDeleteProgram(m_RendererID);

        // Provide the infolog in whatever manner you deem best.
        for (auto it = infoLog.begin(); it != infoLog.end(); ++it) {
            std::cout << *it;
        }
        std::cout << std::endl;

        // Exit with failure.
        return;
    }
    // Always detach shaders after a successful link.
    glDetachShader(m_RendererID, vs);
    glDetachShader(m_RendererID, fs);
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

int Shader::GetUniformLocation(const std::string& name)
{
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1) {
        std::cout << "Warning: uniform " << name << " doesn't exist!" << std::endl;
    }
    return location;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& filepath)
{
    std::ifstream stream(filepath);
    std::string line;
    std::string s(std::istreambuf_iterator<char>(stream), {});
    std::cout << s << std::endl;
    const char* src = s.c_str();

    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length];
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader. Log:" << std::endl;
        std::cout << message << std::endl;
        delete[] message;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

