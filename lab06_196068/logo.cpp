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

	GLFWwindow* window = glfwCreateWindow(800, 600, "FINKI Logo 3D", NULL, NULL);
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
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	float O_offset = 0.3f;
	float Z_offset = 0.3f;

	const float PI = 3.141562f;
	float increment = PI / 180.0f;
	float angle = 0.0f;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	unsigned int index = 0;
	for (int i = 0; i < 361; i++)
	{
		// front
		vertices.push_back(O_offset + 0.35f * cos(angle));
		vertices.push_back(0.35f * sin(angle));
		vertices.push_back(0.0f);

		vertices.push_back(O_offset + 0.6f * cos(angle));
		vertices.push_back(0.6f * sin(angle));
		vertices.push_back(0.0f);

		vertices.push_back(O_offset + 0.6f * cos(angle + increment));
		vertices.push_back(0.6f * sin(angle + increment));
		vertices.push_back(0.0f);

		vertices.push_back(O_offset + 0.35f * cos(angle + increment));
		vertices.push_back(0.35f * sin(angle + increment));
		vertices.push_back(0.0f);

		// back
		vertices.push_back(O_offset + 0.35f * cos(angle));
		vertices.push_back(0.35f * sin(angle));
		vertices.push_back(Z_offset);

		vertices.push_back(O_offset + 0.6f * cos(angle));
		vertices.push_back(0.6f * sin(angle));
		vertices.push_back(Z_offset);

		vertices.push_back(O_offset + 0.6f * cos(angle + increment));
		vertices.push_back(0.6f * sin(angle + increment));
		vertices.push_back(Z_offset);

		vertices.push_back(O_offset + 0.35f * cos(angle + increment));
		vertices.push_back(0.35f * sin(angle + increment));
		vertices.push_back(Z_offset);

		// top
		vertices.push_back(O_offset + 0.6f * cos(angle));
		vertices.push_back(0.6f * sin(angle));
		vertices.push_back(0.0f);

		vertices.push_back(O_offset + 0.6f * cos(angle + increment));
		vertices.push_back(0.6f * sin(angle + increment));
		vertices.push_back(0.0f);
		
		vertices.push_back(O_offset + 0.6f * cos(angle + increment));
		vertices.push_back(0.6f * sin(angle + increment));
		vertices.push_back(Z_offset);
		
		vertices.push_back(O_offset + 0.6f * cos(angle));
		vertices.push_back(0.6f * sin(angle));
		vertices.push_back(Z_offset);

		// bottom
		vertices.push_back(O_offset + 0.35f * cos(angle));
		vertices.push_back(0.35f * sin(angle));
		vertices.push_back(0.0f);

		vertices.push_back(O_offset + 0.35f * cos(angle + increment));
		vertices.push_back(0.35f * sin(angle + increment));
		vertices.push_back(0.0f);

		vertices.push_back(O_offset + 0.35f * cos(angle + increment));
		vertices.push_back(0.35f * sin(angle + increment));
		vertices.push_back(Z_offset);

		vertices.push_back(O_offset + 0.35f * cos(angle));
		vertices.push_back(0.35f * sin(angle));
		vertices.push_back(Z_offset);
		angle += increment;

		std::vector<unsigned int> newIndices
		{
			index, index + 1, index + 2,
			index + 2, index + 3, index,

			index + 4, index + 5, index + 6,
			index + 6, index + 7, index + 4,

			index + 8, index + 9, index + 10,
			index + 10, index + 11, index + 8,

			index + 12, index + 13, index + 14,
			index + 14, index + 15, index + 12
		};
		indices.insert(indices.end(), newIndices.begin(), newIndices.end());
		index += 16;
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

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	std::vector<float> vertices2
	{
		// front
		-0.85f,-0.6f, 0.0f,
		-0.5f, -0.6f, 0.0f,
		-0.5f,  0.6f, 0.0f,
		-0.5f,  0.6f, 0.0f,
		-0.85f, 0.6f, 0.0f,
		-0.85f, -0.6f, 0.0f,

		// back
		-0.85f,-0.6f,  Z_offset,
		-0.5f, -0.6f,  Z_offset,
		-0.5f,  0.6f,  Z_offset,
		-0.5f,  0.6f,  Z_offset,
		-0.85f, 0.6f,  Z_offset,
		-0.85f, -0.6f, Z_offset,

		// left
		-0.85f,-0.6f, 0.0f,
		-0.85f,-0.6f, Z_offset,
		-0.85f, 0.6f, Z_offset,
		-0.85f, 0.6f, Z_offset,
		-0.85f, 0.6f, 0.0f,
		-0.85f,-0.6f, 0.0f,

		// right
		-0.5f,-0.6f, 0.0f,
		-0.5f,-0.6f, Z_offset,
		-0.5f, 0.6f, Z_offset,
		-0.5f, 0.6f, Z_offset,
		-0.5f, 0.6f, 0.0f,
		-0.5f,-0.6f, 0.0f,

		// bottom
		-0.85f,-0.6f, 0.0f,
		-0.5f, -0.6f, 0.0f,
		-0.5f, -0.6f, Z_offset,
		-0.5f, -0.6f, Z_offset,
		-0.85f, -0.6f, Z_offset,
		-0.85f, -0.6f, 0.0f,

		// top
		-0.85f, 0.6f, 0.0f,
		-0.5f, 0.6f, 0.0f,
		-0.5f, 0.6f, Z_offset,
		-0.5f, 0.6f, Z_offset,
		-0.85f, 0.6f, Z_offset,
		-0.85f, 0.6f, 0.0f
	};

	unsigned int VAO2;
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);

	unsigned int VBO2;
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, vertices2.size() * sizeof(float), vertices2.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	char vertexSource[] =
		"#version 330 core\n"
		"layout (location = 0) in vec3 apos;\n"
		"out vec3 col;\n"
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

		glUniform3f(glGetUniformLocation(shader, "col"), 0.164f, 0.6f, 0.82f);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, NULL);

		glUniform3f(glGetUniformLocation(shader, "col"), 0.17f, 0.17f, 0.80f);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, (int)vertices2.size() / 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}