#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <inttypes.h>
#include <math.h>
#include <vector>
#include "stb_image.h"
#include "resource.h"

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int Texture::NUM_TEXTURES_LOADED = 0;

int main()
{
	// Inicijaliziranje na GLFW
	if (glfwInit() == GLFW_FALSE)
	{
		printf("Greska pri inicijaliziranje na GLFW!\n");
		return -1;
	}
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Sozdavanje prozorec i context
	int w = 800, h = 800;
	GLFWwindow* window = glfwCreateWindow(w, h, "Lab04 - Stefan Ivanovski 196068", nullptr, nullptr);
	if (window == nullptr)
	{
		printf("Greska pri kreiranje GLFW prozorec!\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

	// GLAD init
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return -1;
	}
	glViewport(0, 0, w, h);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	float vertices[]
	{
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f
	};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);

	// Pass position and color data to array buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(float), vertices, GL_STATIC_DRAW);

	// Set up attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	const char* vertexSource =
		"#version 330 core\n"
		"layout(location = 0) in vec3 vertex_pos;\n"
		"layout(location = 1) in vec2 uvs;\n"
		"out vec2 uvsout;"
		"void main()\n"
		"{\n"
		"	uvsout = uvs;"
		"	gl_Position = vec4(vertex_pos, 1.0);\n"
		"}\n";

	const char* fragmentSource =
		"#version 330 core\n"
		"out vec4 color;\n"
		"in vec2 uvsout;\n"
		"uniform vec2 size;\n"
		"uniform float rows;\n"
		"void main()\n"
		"{\n"
		"	vec2 uv = floor(rows * gl_FragCoord.xy * vec2(size.x / size.y, 1) / size.xy);\n"
		"	color = vec4(vec3(mod(uv.x + uv.y, 2.)), 1);\n"
		"}\n";

	unsigned int shaderProgram = CreateShader(vertexSource, fragmentSource);
	glUseProgram(shaderProgram);

	int loc1 = glGetUniformLocation(shaderProgram, "size");
	glUniform2f(loc1, (float)w, (float)h);

	int loc2 = glGetUniformLocation(shaderProgram, "rows");
	int brojNaRediciIkoloni = 10;
	glUniform1f(loc2, (float)brojNaRediciIkoloni);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}