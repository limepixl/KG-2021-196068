#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "resource.h"
#include <cstdio>
#include <vector>

int Texture::NUM_TEXTURES_LOADED = 0;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(800, 600, "3D Pacman", NULL, NULL);
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
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	float Z_offset = 0.3f;

	const float PI = 3.141562f;
	float increment = PI / 180.0f;
	float angleOffset = PI / 7.0f;
	float angle = angleOffset;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	unsigned int index = 0;
	for (int i = 0; i < 361; i++)
	{
		if (angle >= 1.7 * PI + angleOffset)
		{
			// mouth top
			vertices.push_back(0.6f * cos(angleOffset));
			vertices.push_back(0.6f * sin(angleOffset));
			vertices.push_back(0.0f);
			vertices.push_back(0.6f * cos(angleOffset));
			vertices.push_back(0.6f * sin(angleOffset));
			vertices.push_back(Z_offset);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(Z_offset);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(Z_offset);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(0.6f * cos(angleOffset));
			vertices.push_back(0.6f * sin(angleOffset));
			vertices.push_back(0.0f);

			// mouth bottom
			vertices.push_back(0.6f * cos(angle - increment));
			vertices.push_back(0.6f * sin(angle - increment));
			vertices.push_back(0.0f);
			vertices.push_back(0.6f * cos(angle - increment));
			vertices.push_back(0.6f * sin(angle - increment));
			vertices.push_back(Z_offset);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(Z_offset);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(Z_offset);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(0.6f * cos(angle - increment));
			vertices.push_back(0.6f * sin(angle - increment));
			vertices.push_back(0.0f);

			break;
		}

		// front
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.6f * cos(angle));
		vertices.push_back(0.6f * sin(angle));
		vertices.push_back(0.0f);
		vertices.push_back(0.6f * cos(angle + increment));
		vertices.push_back(0.6f * sin(angle + increment));
		vertices.push_back(0.0f);

		// back
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(Z_offset);
		vertices.push_back(0.6f * cos(angle));
		vertices.push_back(0.6f * sin(angle));
		vertices.push_back(Z_offset);
		vertices.push_back(0.6f * cos(angle + increment));
		vertices.push_back(0.6f * sin(angle + increment));
		vertices.push_back(Z_offset);

		// top
		vertices.push_back(0.6f * cos(angle));
		vertices.push_back(0.6f * sin(angle));
		vertices.push_back(0.0f);
		vertices.push_back(0.6f * cos(angle));
		vertices.push_back(0.6f * sin(angle));
		vertices.push_back(Z_offset);
		vertices.push_back(0.6f * cos(angle + increment));
		vertices.push_back(0.6f * sin(angle + increment));
		vertices.push_back(Z_offset);
		vertices.push_back(0.6f * cos(angle + increment));
		vertices.push_back(0.6f * sin(angle + increment));
		vertices.push_back(Z_offset);
		vertices.push_back(0.6f * cos(angle + increment));
		vertices.push_back(0.6f * sin(angle + increment));
		vertices.push_back(0.0f);
		vertices.push_back(0.6f * cos(angle));
		vertices.push_back(0.6f * sin(angle));
		vertices.push_back(0.0f);

		angle += increment;
	}

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	char vertexSource[] =
		"#version 330 core\n"
		"layout (location = 0) in vec3 apos;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main() {\n"
		"gl_Position = projection * view * model * vec4(apos, 1.0);\n}";

	char fragmentSource[] =
		"#version 330 core\n"
		"out vec4 color;\n"
		"uniform vec3 col;\n"
		"void main() {\n"
		"color = vec4(col, 1.0);}\n";

	unsigned int shader = CreateShader(vertexSource, fragmentSource);
	glUseProgram(shader);

	glm::mat4 view(1.0f);
	view = glm::translate(view, glm::vec3(0.0, 0.0, -3.0));
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &view[0][0]);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &projection[0][0]);

	int modelLoc = glGetUniformLocation(shader, "model");
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model(1.0f);
		model = glm::rotate(model, glm::radians(100.0f * (float)glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

		glUniform3f(glGetUniformLocation(shader, "col"), 1.0f, 1.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, (int)vertices.size() / 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}