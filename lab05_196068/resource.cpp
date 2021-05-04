#include "resource.h"
#include <glad/glad.h>
#include <cstdio>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

Texture LoadTextureFromFile(const char* path)
{
	Texture result = { 0 };
	stbi_set_flip_vertically_on_load(1);
	unsigned char* data = stbi_load(path, &result.width, &result.height, &result.channels, 0);
	if(data == NULL)
	{
		printf("Greska pri vcituvanje na texture datoteka: %s\n", path);
		exit(-1);
	}
	
	glGenTextures(1, &(result.ID));
	result.index = Texture::NUM_TEXTURES_LOADED++;

	glActiveTexture(GL_TEXTURE0 + result.index);
	glBindTexture(GL_TEXTURE_2D, result.ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	switch (result.channels)
	{
	case 1:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, result.width, result.height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
		break;

	case 2:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8, result.width, result.height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
		break;

	case 3:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, result.width, result.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		break;
	
	case 4:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, result.width, result.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		break;

	default:
		printf("Greska so brojot na kanali na teksturata! %d\n", result.channels);
	}

	return result;
}
