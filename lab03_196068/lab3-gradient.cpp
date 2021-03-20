#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <inttypes.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <sstream>

float map(float value, float min, float max, float newMin, float newMax)
{
	return (value - min) / (max - min) * (newMax - newMin) + newMin;
}

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

unsigned int CreateShader(const char* vertexSource, const char* fragmentSource)
{
	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexSource, nullptr);
	glCompileShader(vertex);

	GLint compiled;
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &compiled);
	if (compiled != GL_TRUE)
	{
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetShaderInfoLog(vertex, 1024, &log_length, message);
		printf("VERTEX SHADER ERROR\n%s\n", message);
	}

	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentSource, nullptr);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &compiled);
	if (compiled != GL_TRUE)
	{
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetShaderInfoLog(fragment, 1024, &log_length, message);
		printf("FRAGMENT SHADER ERROR\n%s\n", message);
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertex);
	glAttachShader(shaderProgram, fragment);
	glLinkProgram(shaderProgram);

	return shaderProgram;
}

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
	int w = 800, h = 600;
	GLFWwindow* window = glfwCreateWindow(w, h, "Lab03 - Stefan Ivanovski 196068", nullptr, nullptr);
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

	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	float y = 1.0f;
	float ystep = 0.666f;
	int index = 0;

	// Crven gradient
	for (int i = 0; i < 10; i++)
	{
		float boja = map((float)i, 0.0f, 9.0f, 0.0f, 1.0f);
		float startX = map((float)i, 0.0f, 10.0f, -1.0f, 1.0f);
		float endX = map((float)i + 1.0f, 0.0f, 10.0f, -1.0f, 1.0f);

		vertices.push_back(startX);
		vertices.push_back(y - ystep);
		vertices.push_back(0.0f);
		vertices.push_back(boja);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);

		vertices.push_back(endX);
		vertices.push_back(y - ystep);
		vertices.push_back(0.0f);
		vertices.push_back(boja);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);

		vertices.push_back(endX);
		vertices.push_back(y);
		vertices.push_back(0.0f);
		vertices.push_back(boja);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);

		vertices.push_back(startX);
		vertices.push_back(y);
		vertices.push_back(0.0f);
		vertices.push_back(boja);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);

		indices.push_back(index);
		indices.push_back(index+1);
		indices.push_back(index+2);
		indices.push_back(index+2);
		indices.push_back(index+3);
		indices.push_back(index);
		index += 4;
	}

	y -= ystep;

	// Zelen gradient
	for (int i = 0; i < 10; i++)
	{
		float boja = map((float)i, 0.0f, 9.0f, 0.0f, 1.0f);
		float startX = map((float)i, 0.0f, 10.0f, -1.0f, 1.0f);
		float endX = map((float)i + 1.0f, 0.0f, 10.0f, -1.0f, 1.0f);

		vertices.push_back(startX);
		vertices.push_back(y - ystep);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(boja);
		vertices.push_back(0.0f);

		vertices.push_back(endX);
		vertices.push_back(y - ystep);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(boja);
		vertices.push_back(0.0f);

		vertices.push_back(endX);
		vertices.push_back(y);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(boja);
		vertices.push_back(0.0f);

		vertices.push_back(startX);
		vertices.push_back(y);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(boja);
		vertices.push_back(0.0f);

		indices.push_back(index);
		indices.push_back(index + 1);
		indices.push_back(index + 2);
		indices.push_back(index + 2);
		indices.push_back(index + 3);
		indices.push_back(index);
		index += 4;
	}

	y -= ystep;

	// Sin gradient
	for (int i = 0; i < 10; i++)
	{
		float boja = map((float)i, 0.0f, 9.0f, 0.0f, 1.0f);
		float startX = map((float)i, 0.0f, 10.0f, -1.0f, 1.0f);
		float endX = map((float)i + 1.0f, 0.0f, 10.0f, -1.0f, 1.0f);

		vertices.push_back(startX);
		vertices.push_back(y - ystep);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(boja);

		vertices.push_back(endX);
		vertices.push_back(y - ystep);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(boja);

		vertices.push_back(endX);
		vertices.push_back(y);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(boja);

		vertices.push_back(startX);
		vertices.push_back(y);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(boja);

		indices.push_back(index);
		indices.push_back(index + 1);
		indices.push_back(index + 2);
		indices.push_back(index + 2);
		indices.push_back(index + 3);
		indices.push_back(index);
		index += 4;
	}

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);

	// Pass position and color data to array buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	// Set up attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	const char* vertexSource =
		"#version 330 core\n"
		"layout(location = 0) in vec3 vertex_pos;\n"
		"layout(location = 1) in vec3 vertex_color;\n"
		"out vec3 col;\n"
		"void main()\n"
		"{\n"
		"   col = vertex_color;\n"
		"	gl_Position = vec4(vertex_pos, 1.0);\n"
		"}\n";

	const char* fragmentSource =
		"#version 330 core\n"
		"in vec3 col;\n"
		"out vec4 color;\n"
		"void main()\n"
		"{\n"
		"	color = vec4(col, 1.0);\n"
		"}\n";

	unsigned int shaderProgram = CreateShader(vertexSource, fragmentSource);
	glUseProgram(shaderProgram);

	int location = glGetUniformLocation(shaderProgram, "col");

	int vertexCount = (int)vertices.size() / 3;
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glUniform3f(location, 0.164f, 0.6f, 0.82f);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}