#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "resource.h"
#include <cstdio>

int Texture::NUM_TEXTURES_LOADED = 0;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Cube", NULL, NULL);
	if (!window)
	{
		printf("Failed to create window!\n");
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to load GLAD!\n");
		return -1;
	}
	glViewport(0, 0, 800, 600);
	glClearColor(0.0f, 0.0f, 0.6f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	float data[]
	{
		// front
		-1.0, -1.0,  1.0,  1.0,  0.0,  1.0,
		 1.0, -1.0,  1.0,  1.0,  0.0,  0.0,
		 1.0,  1.0,  1.0,  1.0,  1.0,  0.0,
		-1.0,  1.0,  1.0,  1.0,  1.0,  1.0,
		// back
		-1.0, -1.0, -1.0,  0.0,  0.0,  1.0,
		 1.0, -1.0, -1.0,  0.0,  0.0,  0.0,
		 1.0,  1.0, -1.0,  0.0,  1.0,  0.0,
		-1.0,  1.0, -1.0,  0.0,  1.0,  1.0
	};

	unsigned int indices[]
	{
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 48 * sizeof(float), data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	char vertexSource[] =
		"#version 330 core\n"
		"layout (location = 0) in vec3 apos;\n"
		"layout (location = 1) in vec3 acolor;\n"
		"out vec3 col;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main() {\n"
		"col = acolor;\n"
		"gl_Position = projection * view * model * vec4(apos, 1.0);\n}";

	char fragmentSource[] =
		"#version 330 core\n"
		"out vec4 color;\n"
		"in vec3 col;\n"
		"void main() {\n"
		"color = vec4(col, 1.0);}\n";

	unsigned int shader = CreateShader(vertexSource, fragmentSource);
	glUseProgram(shader);

	glm::mat4 view(1.0f);
	view = glm::translate(view, glm::vec3(0.0, 0.0, -5.0));
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &view[0][0]);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &projection[0][0]);

	int modelLoc = glGetUniformLocation(shader, "model");
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model(1.0f);
		model = glm::rotate(model, glm::radians(100.0f * (float)glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}