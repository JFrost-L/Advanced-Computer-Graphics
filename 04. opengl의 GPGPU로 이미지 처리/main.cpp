#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GL/glut.h>
#include<algorithm>

#include <FreeImage.h>

using namespace std;

int keyboard=0; // �ʱ� �� ����
float blurAmount = 0.0f; // �ʱ� �� ����
GLuint textureID;
GLuint shaderProgram, edgeShaderProgram;
GLuint VertexArrayID;
GLint textureUniform, blurUniform, keyboardUniform;
GLuint framebuffer;
GLuint edgeTexture;

void processSpecialKeys(int key, int x, int y) {
	if (key == GLUT_KEY_F1) {
		blurAmount = 0.0; // �� ���� �ʱ�ȭ
		keyboard = -1;
	}
	glutPostRedisplay();
}

void processNormalKeys(unsigned char key, int x, int y) {
	if (key == 'q' || key == 'Q') {
		blurAmount = 1.0f; // 'q' Ű�� ������ �� ������ 1.0���� ����
		keyboard = 0;
	}
	else if (key == 'w' || key == 'W') {
		blurAmount = 1.0f;
		keyboard = 1;
	}
	else if (key == 'e' || key == 'E') {
		keyboard = 2;
	}
	glutPostRedisplay();
}

GLuint LoadTexture(const char* imageFilePath) {
	FIBITMAP* image = FreeImage_Load(FIF_PNG, imageFilePath, PNG_DEFAULT);
	if (!image) {
		std::cerr << "Failed to load image: " << imageFilePath << std::endl;
		return 0;
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int width = FreeImage_GetWidth(image);
	int height = FreeImage_GetHeight(image);

	if (FreeImage_GetBPP(image) == 24) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, FreeImage_GetBits(image));
	}
	else if (FreeImage_GetBPP(image) == 32) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, FreeImage_GetBits(image));
	}
	else {
		std::cerr << "Unsupported image format" << std::endl;
		return 0;
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	FreeImage_Unload(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path)
{
	//create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	//Read the vertex shader code from the file
	string VertexShaderCode;
	ifstream VertexShaderStream(vertex_file_path, ios::in);
	if (VertexShaderStream.is_open())
	{
		string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}

	//Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	//Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength != 0) {
		vector<char> VertexShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
	}
	//Read the fragment shader code from the file
	string FragmentShaderCode;
	ifstream FragmentShaderStream(fragment_file_path, ios::in);
	if (FragmentShaderStream.is_open())
	{
		string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	//Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	//Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength != 0) {
		vector<char> FragmentShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
	}
	//Link the program
	fprintf(stdout, "Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength != 0) {
		vector<char> ProgramErrorMessage(max(InfoLogLength, int(1)));
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
	}
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


void init()
{
	// GLEW �ʱ�ȭ
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		std::cerr << "Error: " << glewGetErrorString(res) << std::endl;
		exit(EXIT_FAILURE);
	}

	// �̹����� �ε��ϰ� �ؽ�ó�� ����
	textureID = LoadTexture("Image.png");
	// VAO ���� �� ���ε�
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// ���� ������ ���� (���簢���� ���� ��ġ�� �ؽ�ó ��ǥ)
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // ���� �Ʒ�
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // ������ �Ʒ�
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // ������ ��
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f   // ���� ��
	};

	// VBO ���� �� ���ε�
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// ���� �Ӽ� ������ ����
	// (��ġ �Ӽ� �� �ؽ�ó ��ǥ �Ӽ�)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// ���̴� ���α׷� ���� �� ���
	edgeShaderProgram = LoadShaders("VertexShader.txt", "FragmentShader.txt");
	shaderProgram = LoadShaders("VertexShader.txt", "FragmentShader.txt");
	glUseProgram(shaderProgram);

	// �ؽ�ó ������ ����
	textureUniform = glGetUniformLocation(shaderProgram, "textureSampler");
	glUniform1i(textureUniform, 0);  // �ؽ�ó ����Ʈ 0�� ���

	// �ؽ�ó ���ε�
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void renderScene(void) {
	if (keyboard != 2) {
		// ȭ�� �����
		glClear(GL_COLOR_BUFFER_BIT);

		// �ؽ�ó ���ε�
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// ���̴� ���α׷��� ���
		glUseProgram(shaderProgram);

		// �ؽ�ó ������ ����
		textureUniform = glGetUniformLocation(shaderProgram, "textureSampler");
		glUniform1i(textureUniform, 0);  // �ؽ�ó ����Ʈ 0�� ���

		glutSpecialFunc(processSpecialKeys);
		glutKeyboardFunc(processNormalKeys);

		// �� ���� ����
		blurUniform = glGetUniformLocation(shaderProgram, "blurAmount");
		glUniform1f(blurUniform, blurAmount);

		// VAO ���ε�
		glBindVertexArray(VertexArrayID);

		// ���簢�� �׸���
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
	// E Ű�� ������ �� ���� ���� ����
	else if (keyboard == 2) {
		printf("edgeShaderProgram");
		// FBO ���ε�
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		// ���� ������ ���� fragment shader ���
		glUseProgram(edgeShaderProgram);

		textureUniform = glGetUniformLocation(edgeShaderProgram, "textureSampler");
		glUniform1i(textureUniform, 0);

		blurUniform = glGetUniformLocation(edgeShaderProgram, "blurAmount");
		glUniform1f(blurUniform, blurAmount);

		keyboardUniform = glGetUniformLocation(edgeShaderProgram, "keyboard");
		glUniform1i(keyboardUniform, keyboard);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glBindVertexArray(VertexArrayID);

		// ���� ���� ����� FBO�� ������
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	}
	// ���� ����
	glutSwapBuffers();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(480, 480);
	glutCreateWindow("OpenGL Texture Mapping");

	init();

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	glutDisplayFunc(renderScene);
	glutMainLoop();

	glDeleteVertexArrays(1, &VertexArrayID);

	return 0;
}

