#define uint unsigned int

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"


int main()
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW failed..." << std::endl;
		return -1;
	}

	// During init, enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	float positions[8]{
		-0.5f, -0.5,
		0.5f, -0.5f,
		0.5f, 0.5f,
		-0.5f, 0.5f };

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0 };

	VertexArray va{};
	VertexBuffer vb{ positions, 4 * 2 * sizeof(float) };
	VertexBufferLayout layout{};
	layout.Push(GL_FLOAT, 2);
	va.AddBuffer(vb, layout);

	IndexBuffer ib{ indices, 6 };

	Shader shader{ "res/shaders/basic.vert", "res/shaders/basic.frag" };

	float r = 0.0f;
	float increment = 0.05f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		// This is what we do before a draw call!!
		shader.Bind();
		shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

		va.Bind();
		ib.Bind();
		// Then we draw
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); // last parameter is buffer to draw from - ibo is already bound however

		if (r > 1.0f || r < 0.0f) {
			increment *= -1;
		}

		r += increment;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}