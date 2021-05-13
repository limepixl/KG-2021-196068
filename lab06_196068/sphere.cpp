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

	GLFWwindow* window = glfwCreateWindow(800, 600, "3D Sphere", NULL, NULL);
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

	// params
	// http://www.songho.ca/opengl/gl_sphere.html
	const float PI = 3.141592f;
	int numSectors = 100;
	int numStacks = 100;
	float radius = 1.0f;
	float sectorStep = 2.0f * PI / numSectors;
	float stackStep = PI / numStacks;

	std::vector<float> vertices;
	for (int i = 0; i <= numStacks; i++)
	{
		float stackAngle = PI / 2.0f - i * stackStep;
		float xy = radius * cosf(stackAngle);
		float z = radius * sinf(stackAngle);

		for (int j = 0; j <= numSectors; j++)
		{
			float sectorAngle = j * sectorStep;
			float x = xy * cosf(sectorAngle);
			float y = xy * sinf(sectorAngle);
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
	}

	std::vector<unsigned int> indices;
	for (int i = 0; i < numStacks; ++i)
	{
		int k1 = i * (numSectors + 1);
		int k2 = k1 + numSectors + 1;

		for (int j = 0; j < numSectors; j++, k1++, k2++)
		{
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if (i != (numStacks - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	char vertexSource[] =
		"#version 330 core\n"
		"layout (location = 0) in vec3 apos;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"out vec4 fragPos;\n"
		"void main() {\n"
		"fragPos = model * vec4(apos, 1.0);\n"
		"gl_Position = projection * view * model * vec4(apos, 1.0);\n}";

	char fragmentSource[] =
		"#version 330 core\n"
		"in vec4 fragPos;\n"
		"out vec4 color;\n"
		"uniform vec3 col;\n"
		"void main() {\n"
		"color = vec4(fragPos.xyz, 1.0);}\n";

	unsigned int shader = CreateShader(vertexSource, fragmentSource);
	glUseProgram(shader);
	glUniform3f(glGetUniformLocation(shader, "col"), 1.0f, 0.0f, 0.0f);

	glm::mat4 view(1.0f);
	view = glm::translate(view, glm::vec3(0.0, 0.0, -5.0));
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &view[0][0]);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &projection[0][0]);

	int modelLoc = glGetUniformLocation(shader, "model");
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model(1.0f);
		model = glm::rotate(model, glm::radians(100.0f * (float)glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}