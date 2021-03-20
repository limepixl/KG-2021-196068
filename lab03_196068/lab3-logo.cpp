#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <inttypes.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <sstream>

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
	int w = 800, h = 800;
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

	float O_offset = 0.3f;

	const float PI = 3.141562f;
	float increment = PI / 180.0f;
	float angle = 0.0f;
	std::vector<float> vertices;
	vertices.push_back(O_offset);
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);
	for (int i = 0; i < 361; i++)
	{
		vertices.push_back(O_offset + 0.6f * cos(angle));
		vertices.push_back(0.6f * sin(angle));
		vertices.push_back(0.0f);
		angle += increment;
	}

	std::vector<float> innerCircle;
	angle = 0.0f;
	innerCircle.push_back(O_offset);
	innerCircle.push_back(0.0f);
	innerCircle.push_back(0.0f);
	for (int i = 0; i < 361; i++)
	{
		innerCircle.push_back(O_offset + 0.35f * cos(angle));
		innerCircle.push_back(0.35f * sin(angle));
		innerCircle.push_back(0.0f);
		angle += increment;
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	GLuint VAO2;
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);

	GLuint VBO2;
	glGenBuffers(1, &VBO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, innerCircle.size() * sizeof(float), innerCircle.data(), GL_STATIC_DRAW);

	// Set up attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// Set up attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	float ivertices[]
	{
		-0.85f,-0.6f, 0.0f,
		-0.5f, -0.6f, 0.0f,
		-0.5f,  0.6f, 0.0f,
		-0.5f,  0.6f, 0.0f,
		-0.85f, 0.6f, 0.0f,
		-0.85f -0.6f, 0.0f
	};

	GLuint VAO3;
	glGenVertexArrays(1, &VAO3);
	glBindVertexArray(VAO3);

	GLuint VBO3;
	glGenBuffers(1, &VBO3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ivertices), ivertices, GL_STATIC_DRAW);

	// Set up attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// Set up attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	const char* vertexSource =
		"#version 330 core\n"
		"layout(location = 0) in vec3 vertex_pos;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(vertex_pos, 1.0);\n"
		"}\n";

	const char* fragmentSource =
		"#version 330 core\n"
		"out vec4 color;\n"
		"uniform vec3 col;\n"
		"void main()\n"
		"{\n"
		"	color = vec4(col, 1.0);\n"
		"}\n";

	unsigned int shaderProgram = CreateShader(vertexSource, fragmentSource);
	glUseProgram(shaderProgram);

	int location = glGetUniformLocation(shaderProgram, "col");

	int vertexCount = (int)vertices.size() / 3;
	int vertexCount2 = (int)innerCircle.size() / 3;
	int vertexCount3 = sizeof(ivertices) / sizeof(float) / 3.0f;
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glUniform3f(location, 0.164f, 0.6f, 0.82f);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);

		glUniform3f(location, 1.0f, 1.0f, 1.0f);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount2);

		glUniform3f(location, 0.17f, 0.17f, 0.80f);
		glBindVertexArray(VAO3);
		glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}