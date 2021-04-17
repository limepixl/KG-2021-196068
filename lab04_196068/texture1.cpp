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
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.5f, 1.0f
	};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);

	// Pass position and color data to array buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(float), vertices, GL_STATIC_DRAW);

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
		"in vec2 uvsout;"
		"uniform sampler2D tex;"
		"void main()\n"
		"{\n"
		"	color = texture(tex, uvsout);\n"
		"}\n";

	unsigned int shaderProgram = CreateShader(vertexSource, fragmentSource);
	glUseProgram(shaderProgram);

	int location = glGetUniformLocation(shaderProgram, "tex");

	// NOTE: ova e samo koga koristite Visual Studio
	Texture checkerboard = LoadTextureFromFile("../../../checkerboard.jpg");
	glActiveTexture(GL_TEXTURE0 + checkerboard.index);
	glBindTexture(GL_TEXTURE_2D, checkerboard.ID);

	glUniform1i(location, checkerboard.index);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}