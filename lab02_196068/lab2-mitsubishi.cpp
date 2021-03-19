#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <inttypes.h>
#include <math.h>

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
	int w = 800, h = 800;
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
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	float vertices[]
	{
		// centralen crven triagolnik
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,

		// lev bel triagolnik
		-0.333f, -0.1666f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, -0.1666f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.1666, 0.1666f, 0.0f, 1.0f, 1.0f, 1.0f,

		// desen bel triagolnik
		0.333f, -0.1666f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, -0.1666f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.1666f, 0.1666f, 0.0f, 1.0f, 1.0f, 1.0f,

		// dolen bel triagolnik
		-0.1666, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.1666, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, -0.1666, 0.0f, 1.0f, 1.0f, 1.0f
	};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); 

	GLuint VBO;
	glGenBuffers(1, &VBO);

	// Pass position and color data to array buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set up attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	const char* vertexSource = 
	"#version 330 core\n"
	"layout(location = 0) in vec3 vertex_pos;\n"
	"layout(location = 1) in vec3 vertex_color;\n"
	"out vec3 col;\n"
	"void main(){\n"
	"col = vertex_color;\n"
	"gl_Position = vec4(vertex_pos, 1.0);\n"
	"}\n";

	const char* fragmentSource =
	"#version 330 core\n"
	"in vec3 col;"
	"out vec4 color;\n"
	"void main(){\n"
	"color = vec4(col, 1.0);\n"
	"}\n";

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, nullptr);
	glCompileShader(vertexShader);

	// Detekcija na errors
	GLint compiled;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
	if(compiled != GL_TRUE)
	{
	    GLsizei log_length = 0;
	    GLchar message[1024];
	    glGetShaderInfoLog(vertexShader, 1024, &log_length, message);
	    printf("VERTEX SHADER COMPILE ERROR:\n%s\n", message);
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
	if(compiled != GL_TRUE)
	{
	    GLsizei log_length = 0;
	    GLchar message[1024];
	    glGetShaderInfoLog(fragmentShader, 1024, &log_length, message);
	    printf("FRAGMENT SHADER COMPILE ERROR:\n%s\n", message);
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// clean up
	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgram);

	int vertexCount = sizeof(vertices) / sizeof(float) / 6;
	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, vertexCount);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}