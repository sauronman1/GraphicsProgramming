#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"

using namespace glm;
GLFWwindow* window;

struct Matrix
{
	float m11, m12, m13;
	float m21, m22, m23;
	float m31, m32, m33;

	Matrix operator*(const Matrix& rhs) const
	{
		Matrix result;
		// m11 = this.col(1) * rhs.row(1);
		result.m11 = m11 * rhs.m11 + m21 * rhs.m12 + m31 * rhs.m13;
		// m11 = this.col(2) * rhs.row(1);
		result.m12 = m12 * rhs.m11 + m22 * rhs.m12 + m32 * rhs.m13;
		// m11 = this.col(3) * rhs.row(1);
		result.m13 = m13 * rhs.m11 + m23 * rhs.m12 + m33 * rhs.m13;

		// m11 = this.col(1) * rhs.row(2);
		result.m21 = m11 * rhs.m21 + m21 * rhs.m22 + m31 * rhs.m23;
		// m11 = this.col(2) * rhs.row(2);
		result.m22 = m12 * rhs.m21 + m22 * rhs.m22 + m32 * rhs.m23;
		// m11 = this.col(3) * rhs.row(2);
		result.m23 = m13 * rhs.m21 + m23 * rhs.m22 + m33 * rhs.m23;

		// m11 = this.col(1) * rhs.row(3);
		result.m31 = m11 * rhs.m31 + m21 * rhs.m32 + m31 * rhs.m33;
		// m11 = this.col(2) * rhs.row(3);
		result.m32 = m12 * rhs.m31 + m22 * rhs.m32 + m32 * rhs.m33;
		// m11 = this.col(3) * rhs.row(3);
		result.m33 = m13 * rhs.m31 + m23 * rhs.m32 + m33 * rhs.m33;

		return result;
	}
};

const float triangleA_Data[] =
{
	//position						uv					normals
	-0.5f,	-0.5f,	0.f,			0.f, 0.f,			0.f, 0.f, 1.f,
	 0.5f,	-0.5f,	0.f,			1.f, 0.f,			0.f, 0.f, 1.f,
	 0.5f,	 0.5f,	0.f,			1.f, 1.f,			0.f, 0.f, 1.f,
	 -0.5f,	 0.5f,	0.f,			0.f, 1.f,			0.f, 0.f, 1.f,
};

const int triangleA_Index_Data[] =
{
	0,1,2,
	0,2,3,

	
};

const float cube_Data[] =
{
	//position				//uv				normals
	-0.5f, -0.5f, 0.5f,		0.f, 0.f,			0.f, 0.f, 1.f,
	0.5f, -0.5f, 0.5f,		1.f, 0.f,			0.f, 0.f, 1.f,
	0.5f, 0.5f, 0.5f,		1.f, 1.f,			0.f, 0.f, 1.f,
	-0.5f, 0.5f, 0.5f,		0.f, 1.f,			0.f, 0.f, 1.f,

	0.5f, -0.5f, -0.5f,		0.f, 0.f,			0.f, 0.f, -1.f,
	-0.5f, -0.5f, -0.5f,	1.f, 0.f,			0.f, 0.f, -1.f,
	-0.5f, 0.5f, -0.5f,		1.f, 1.f,			0.f, 0.f, -1.f,
	0.5f, 0.5f, -0.5f,		0.f, 1.f,			0.f, 0.f, -1.f,

	-0.5f, 0.5f, 0.5f,		0.f, 0.f,			0.f, 1.f, 0.f,
	0.5f, 0.5f, 0.5f,		1.f, 0.f,			0.f, 1.f, 0.f,
	0.5f, 0.5f, -0.5f,		1.f, 1.f,			0.f, 1.f, 0.f,
	-0.5f, 0.5f, -0.5f,		0.f, 1.f,			0.f, 1.f, 0.f,

	-0.5f, -0.5f, 0.5f,		0.f, 0.f,			0.f, -1.f, 0.f,
	-0.5f, -0.5f, -0.5f,	1.f, 0.f,			0.f, -1.f, 0.f,
	0.5f, -0.5f, -0.5f,		1.f, 1.f,			0.f, -1.f, 0.f,
	0.5f, -0.5f, 0.5f,		0.f, 1.f,			0.f, -1.f, 0.f,

	0.5f, -0.5f, 0.5f,		0.f, 0.f,			1.f, 0.f, 0.f,
	0.5f, -0.5f, -0.5f,	    1.f, 0.f,			1.f, 0.f, 0.f,
	0.5f, 0.5f, -0.5f,		1.f, 1.f,			1.f, 0.f, 0.f,
	0.5f, 0.5f, 0.5f,		0.f, 1.f,			1.f, 0.f, 0.f,

	-0.5f, -0.5f, 0.5f,		0.f, 0.f,			-1.f, 0.f, 0.f,
	-0.5f, 0.5f, 0.5f,		1.f, 0.f,			-1.f, 0.f, 0.f,
	-0.5f, 0.5f, -0.5f,		1.f, 1.f,			-1.f, 0.f, 0.f,
	-0.5f, -0.5f, -0.5f,	0.f, 1.f,			-1.f, 0.f, 0.f,
};

const int cube_Index_Data[] =
{
	//z+
	0,1,2,
	0,2,3,

	//z-
	4,5,6,
	4,6,7,

	//y+
	8,9,10,
	8,10,11,

	//y-
	12,13,14,
	12,14,15,

	//x+
	16,17,18,
	16,18,19,

	//x+
	20,21,22,
	20,22,23,
};

int wndHeight;
int wndWidth;
void handleWindowResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	wndHeight = height;
	wndWidth = width;
}

void flipTexture(stbi_uc* data, int width, int height, int components)
{
	int lineSize = width * components;

	stbi_uc* buffer = (stbi_uc*)malloc(width * height * components);
	memcpy(buffer, data, width * height * components);

	for (int y = 0; y < height; y++)
	{
		int flippedY = (height - y - 1);
		memcpy(data + lineSize * y, buffer + lineSize *flippedY, lineSize);
	}

	free(buffer);
}

GLuint loadMesh(const void* data, unsigned int size, const void* elementData, unsigned int elementSize)
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(5 * sizeof(float)));

	GLuint elementBuffer;
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementSize, elementData, GL_STATIC_DRAW);

	return vao;
}

GLuint loadShader(GLenum type, const char* path)
{
	FILE* file = NULL;
	fopen_s(&file, path, "rb");
	if (!file)
	{
		printf("Failed to load shader '&s'\n");
		return 0;
	}

	fseek(file, 0, SEEK_END);
	int fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* fileBuffer = (char*)malloc(fileSize);
	fread(fileBuffer, 1, fileSize, file);

	fclose(file);

	static char infoBuffer[1024];

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &fileBuffer, &fileSize);
	glCompileShader(shader);

	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (!compileStatus)
	{
		glGetShaderInfoLog(shader, 1024, NULL, infoBuffer);
		printf("COMPILE ERROR:\n%s\n", infoBuffer);
	}

	return shader;
}

GLuint loadProgram(const char* vertexPath, const char* fragmentPath)
{
	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexPath);
	GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentPath);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glUseProgram(program);

	return program;
}

GLuint loadTexture(const char* path)
{
	//Load Texture from file
	int texWidth;
	int texHeight;
	int texComponents;
	stbi_uc* texPixels = stbi_load(path, &texWidth, &texHeight, &texComponents, 4);
	flipTexture(texPixels, texWidth, texHeight, 4);
	//Upload from OpenGL
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texPixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	free(texPixels);
	return texture;
}

int main()
{
	glfwInit();
	window = glfwCreateWindow(800, 600, "LET THERE BE GRAPHICS!", NULL, NULL);
	handleWindowResize(window, 800, 600);
	glfwMakeContextCurrent(window);
	glewInit();

	glfwSetWindowSizeCallback(window, handleWindowResize);
	
	// Load meshes!
	GLuint triangleA = loadMesh(triangleA_Data, sizeof(triangleA_Data), triangleA_Index_Data, sizeof(triangleA_Index_Data));
	GLuint cube = loadMesh(cube_Data, sizeof(cube_Data), cube_Index_Data, sizeof(cube_Index_Data));
	glBindVertexArray(triangleA);

	//load Textures
	GLuint textureA = loadTexture("textures/wall.jpg");
	GLuint textureB = loadTexture("textures/grass.jpg");

	
	// Shader stuff
	GLuint programA = loadProgram("shaders/test.vert", "shaders/test.frag");
	GLuint programB = loadProgram("shaders/poop.vert", "shaders/poop.frag");
	glUseProgram(programA);

	GLint u_Model = glGetUniformLocation(programA, "u_Model");
	GLint u_Projection = glGetUniformLocation(programA, "u_Projection");
	GLint u_View = glGetUniformLocation(programA, "u_View");
	GLint u_Time = glGetUniformLocation(programA, "u_Time");

	//Camera
	Camera camera;
	camera.position = vec3(0.f, 0.f, 0.5f);

	float time = 0.0f;
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.1f, 0.1f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Time
		time += 0.01f;
		glUniform1f(u_Time, time);

		//Update view + projection
		mat4 identity(1.f);

		float aspect = (float)wndWidth/ (float)wndHeight;
		mat4 projection = perspective(radians(90.f), aspect, 0.1f, 100.f);
		glUniformMatrix4fv(u_Projection, 1, false, (float*)&projection);

		// view matrix
		mat4 view = camera.getViewMatrix();
		glUniformMatrix4fv(u_View, 1, false, (float*)&view);

		//render grass
		mat4 model;// = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, sin(time) * 2.f));
		model = rotate(mat4(1.f), -pi<float>() / 2.f, vec3(1.f, 0.f, 0.f));
		model = scale(model, vec3(50.f));

		glBindVertexArray(triangleA);
		glBindTexture(GL_TEXTURE_2D, textureB);
		glUniformMatrix4fv(u_Model, 1, false, (float*)&model);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		//render wall
		mat4 modelWall;// = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, sin(time) * 2.f));
		modelWall = translate(mat4(1.f), vec3(0.f, 2.f, 0.f)) * rotate(mat4(1.f), time, vec3(1.f, 1.f, 0.f));

		glBindVertexArray(cube);
		glBindTexture(GL_TEXTURE_2D, textureA);
		glUniformMatrix4fv(u_Model, 1, false, (float*)&modelWall);
		glDrawElements(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}