#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <inttypes.h>

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
	GLFWwindow* window = glfwCreateWindow(w, h, "Lab01 - Stefan Ivanovski 196068", nullptr, nullptr);
	if(window == nullptr)
	{
		printf("Greska pri kreiranje GLFW prozorec!\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// GLAD init
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return -1;
	}
	glViewport(0, 0, w, h);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// NOTE: Probav da gi aproksimiram najdobro sto mozev
	float vertices[]
	{
		// vnatreshen zelen cetiriagolnik
		-0.75f,  -0.75f, 0.0f, 0.3f, 0.5f, 0.1f,
		0.0f, -0.75f, 0.0f, 0.3f, 0.5f, 0.1f,
		0.0f, 0.0f, 0.0f, 0.3f, 0.5f, 0.1f,
		-0.75f, 0.0f, 0.0f, 0.3f, 0.5f, 0.1f,

		// desen zelen romb
		0.0f, -0.75f, 0.0f, 0.3f, 0.5f, 0.1f,
		0.9f, -0.333f, 0.0f, 0.3f, 0.5f, 0.1f,
		0.9f, 0.4f, 0.0f, 0.3f, 0.5f, 0.1f,
		0.0f, 0.0f, 0.0f, 0.3f, 0.5f, 0.1f,

		// goren crven romb
		0.0f, 0.0f, 0.0f, 1.0f, 0.2f, 0.3f,
		0.9f, 0.4f, 0.0f, 1.0f, 0.2f, 0.3f,
		0.4f, 0.8f, 0.0f, 1.0f, 0.2f, 0.3f,
		-0.5f, 0.4f, 0.0f, 1.0f, 0.2f, 0.3f,

		// lev crven triagolnik
		-0.75f, 0.0f, 0.0f, 1.0f, 0.2f, 0.3f,
		0.0f, 0.0f, 0.0f, 1.0f, 0.2f, 0.3f,
		-0.5f, 0.4f, 0.0f, 1.0f, 0.2f, 0.3f,
	};

	uint32_t indices[]
	{
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14
	};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); 

	GLuint VBO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Pass position and color data to array buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 90 * sizeof(float), vertices, GL_STATIC_DRAW);

	// Set up attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	// Pass indices to element array buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 21 * sizeof(uint32_t), indices, GL_STATIC_DRAW);

	const char* vertexSource = 
	"#version 330 core\n"
	"layout(location = 0) in vec3 vertex_pos;\n"
	"layout(location = 1) in vec3 vertex_color;\n"
	"out vec3 col;\n"
	"void main(){\n"
	"gl_Position = vec4(vertex_pos, 1.0);\n"
	"col = vertex_color;\n"
	"}\n";

	const char* fragmentSource =
	"#version 330 core\n"
	"in vec3 col;\n"
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

	int vertexCount = sizeof(vertices) / sizeof(float) / 2;
	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}