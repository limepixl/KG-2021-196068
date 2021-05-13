#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "resource.h"
#include <cstdio>
#include <vector>

int Texture::NUM_TEXTURES_LOADED = 0;

struct Camera
{
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 up;
	float pitch;
	float yaw;
};
Camera cam = {};

static float lastX = 400.0f;
static float lastY = 300.0f;

void ProcessInput(GLFWwindow *window, float deltaTime)
{
	const float movementSpeed = 2.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.position += movementSpeed * cam.forward;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.position -= movementSpeed * cam.forward;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.position -= movementSpeed * glm::normalize(glm::cross(cam.forward, cam.up));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.position += movementSpeed * glm::normalize(glm::cross(cam.forward, cam.up));

	if (cam.position.y < 0.0f)
		cam.position.y = 0.0f;
	if (cam.position.y > 0.0f)
		cam.position.y = 0.0f;
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;
	lastX = (float)xpos;
	lastY = (float)ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	cam.yaw += xoffset;
	cam.pitch += yoffset;
	
	if (cam.pitch > 89.0f)
		cam.pitch = 89.0f;
	if (cam.pitch < -89.0f)
		cam.pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
	direction.y = sin(glm::radians(cam.pitch));
	direction.z = sin(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
	cam.forward = glm::normalize(direction);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(800, 600, "3D Camera", NULL, NULL);
	if (!window)
	{
		printf("Failed to create window!\n");
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, MouseCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to load GLAD!\n");
		return -1;
	}
	glViewport(0, 0, 800, 600);
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	cam.position = glm::vec3(0.0f, 0.0f, -3.0f);
	cam.forward = glm::vec3(0.0f, 0.0f, 1.0f);
	cam.up = glm::vec3(0.0f, 1.0f, 0.0f);
	cam.yaw = -90.0f;
	cam.pitch = 0.0f;

	std::vector<float> vertices
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	std::vector<float> floor
	{
		-1.0f, 0.0f, -1.0f,
		1.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, -1.0f
	};

	unsigned int VAO2;
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);

	unsigned int VBO2;
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, floor.size() * sizeof(float), floor.data(), GL_STATIC_DRAW);

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

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &projection[0][0]);

	int modelLoc = glGetUniformLocation(shader, "model");

	// Delta time calc
	float lastTime = 0.0f;
	float deltaTime = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float currentTime = (float)glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		ProcessInput(window, deltaTime);

		glm::mat4 view = glm::lookAt(cam.position, cam.position + cam.forward, cam.up);
		glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &view[0][0]);

		glm::mat4 model(1.0f);
		model = glm::rotate(model, glm::radians(100.0f * (float)glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

		glUniform3f(glGetUniformLocation(shader, "col"), 1.0f, 1.0f, 0.0f);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, (int)vertices.size() / 3);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
		glUniform3f(glGetUniformLocation(shader, "col"), 0.4f, 0.0f, 0.0f);

		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, (int)floor.size() / 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}