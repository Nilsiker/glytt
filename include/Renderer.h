#pragma once
#include <GL/glew.h>
#include <iostream>

struct GLFWInitException : std::exception
{
    const char* what() const noexcept override {
		return "GLFW could not be initialized.";
	}
};

struct GLEWInitException : std::exception
{
    const char* what() const noexcept override {
		return "GLEW could not be initialized. Check your Extension Wrangler setup!";
	}
};

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);