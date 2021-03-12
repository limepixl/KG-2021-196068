#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <inttypes.h>
#include <math.h>
#include <vector>

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	// Inicijaliziranje na GLFW
	if(glfwInit() == GLFW_FALSE)
	{
		printf("Greska pri inicijaliziranje na GLFW!\n");
		return -1;
	}
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Sozdavanje prozorec i context
	int w = 800, h = 600;
	GLFWwindow* window = glfwCreateWindow(w, h, "Lab02 - Stefan Ivanovski 196068", nullptr, nullptr);
	if(window == nullptr)
	{
		printf("Greska pri kreiranje GLFW prozorec!\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

	// GLAD init
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return -1;
	}
	glViewport(0, 0, w, h);
	glClearColor(0.9f, 0.75f, 0.2f, 1.0f);

	const float PI = 3.141562f;
	float increment = PI / 180.0f;
	float angle = 0.0f;
	std::vector<float> vertices;
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);
	for(;;)
	{
		vertices.push_back(0.75f*cos(angle));
		vertices.push_back(0.75f*sin(angle));
		vertices.push_back(0.0f);
		if (angle > PI / 3.0 && angle < 2.0 * PI / 3.0)
		{
			angle = 2.0 * PI / 3.0;
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
		}
		else if (angle > PI && angle < 4.0 * PI / 3.0)
		{
			angle = 4.0 * PI / 3.0;
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
		}
		else if (angle > 5.0 * PI / 3.0 && angle < 2.0 * PI)
			break;
		angle += increment;
	}

	std::vector<float> innerCircle;
	angle = 0.0f;
	innerCircle.push_back(0.0f);
	innerCircle.push_back(0.0f);
	innerCircle.push_back(0.0f);
	for (int i = 0; i < 361; i++)
	{
		innerCircle.push_back(0.2f*cos(angle));
		innerCircle.push_back(0.2f*sin(angle));
		innerCircle.push_back(0.0f);
		angle += increment;
	}

	std::vector<float> mostInner = innerCircle;
	for (auto& f : mostInner)
		f *= 0.7f;

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

	GLuint VAO3;
	glGenVertexArrays(1, &VAO3);
	glBindVertexArray(VAO3);

	GLuint VBO3;
	glGenBuffers(1, &VBO3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, mostInner.size() * sizeof(float), mostInner.data(), GL_STATIC_DRAW);

	// Set up attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	const char* vertexSource = 
	"#version 330 core\n"
	"layout(location = 0) in vec3 vertex_pos;\n"
	"void main(){\n"
	"gl_Position = vec4(vertex_pos, 1.0);\n"
	"}\n";

	const char* fragmentSource =
	"#version 330 core\n"
	"out vec4 color;\n"
	"void main(){\n"
	"color = vec4(0.0, 0.0, 0.0, 1.0);\n"
	"}\n";

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, nullptr);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	const char* fragmentSource2 =
		"#version 330 core\n"
		"out vec4 color;\n"
		"void main(){\n"
		"color = vec4(0.9, 0.75, 0.2, 1.0);\n"
		"}\n";

	GLuint vertexShader2 = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader2, 1, &vertexSource, nullptr);
	glCompileShader(vertexShader2);

	GLuint fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &fragmentSource2, nullptr);
	glCompileShader(fragmentShader2);

	GLuint shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader2);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);

	int vertexCount = (int)vertices.size() / 3;
	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
		glUseProgram(shaderProgram2);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLE_FAN, 0, (int)innerCircle.size() / 3);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO3);
		glDrawArrays(GL_TRIANGLE_FAN, 0, (int)mostInner.size() / 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}