#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "resource.h"
#include <cstdio>

int Texture::NUM_TEXTURES_LOADED = 0;
float rotation;
float scaleX = 1.0f;
float x = 0.0f, y = 0.0f;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(action == GLFW_PRESS)
	if (key == GLFW_KEY_RIGHT)
	{
		rotation = 0.0f;
		scaleX = 1.0f;
		x += 1.0f;
	}
	else if (key == GLFW_KEY_LEFT)
	{
		rotation = 0.0f;
		scaleX = -1.0f;
		x -= 1.0f;
	}
	else if (key == GLFW_KEY_UP)
	{
		rotation = 90.0f;
		scaleX = 1.0f;
		y += 1.0f;
	}
	else if (key == GLFW_KEY_DOWN)
	{
		rotation = 270.0f;
		scaleX = 1.0f;
		y -= 1.0f;
	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Pacman", NULL, NULL);
	if (!window)
	{
		printf("Failed to create window!\n");
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to load GLAD!\n");
		return -1;
	}
	glViewport(0, 0, 800, 600);
	glClearColor(0.0f, 0.0f, 0.6f, 1.0f);

	float data[]
	{
		-0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 0.0f
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	char vertexSource[] =
		"#version 330 core\n"
		"layout (location = 0) in vec2 pos;\n"
		"layout (location = 1) in vec2 uv;\n"
		"out vec2 uv_out;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main() {\n"
		"uv_out = uv;\n"
		"gl_Position = projection * view * model * vec4(pos, 0.0, 1.0);\n}";

	char fragmentSource[] =
		"#version 330 core\n"
		"out vec4 color;\n"
		"in vec2 uv_out;\n"
		"uniform sampler2D tex;\n"
		"void main() {\n"
		"color = texture(tex, uv_out);\n"
		"if(color.a < 0.1) discard;\n}";

	Texture pacmanTexture = LoadTextureFromFile("../../../pacman.png");
	glActiveTexture(GL_TEXTURE0 + pacmanTexture.index);
	glBindTexture(GL_TEXTURE_2D, pacmanTexture.ID);

	unsigned int shader = CreateShader(vertexSource, fragmentSource);
	glUseProgram(shader);
	glUniform1i(glGetUniformLocation(shader, "tex"), pacmanTexture.index);

	glm::mat4 view(1.0f);
	view = glm::translate(view, glm::vec3(0.0, 0.0, -5.0));
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &view[0][0]);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &projection[0][0]);

	int modelLoc = glGetUniformLocation(shader, "model");
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(x, y, 0.0f));
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(scaleX, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}