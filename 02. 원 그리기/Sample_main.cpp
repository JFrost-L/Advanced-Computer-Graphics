#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GL/glut.h>
#include<algorithm>
#include<math.h>

const GLfloat PI = 3.14;

using namespace std;
void init();
void renderScene(void);
GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
vector<GLfloat> vertices;
GLfloat width = 500, height = 500;
int counter = 0;
GLuint programID;
GLint vtxPosition;
GLfloat rx, ry;
GLuint VAO;
GLuint VBO;
GLfloat r = 0;
GLint tessllation = 10;
GLint clickCounter = 0;
GLfloat firstX, firstY, secondX, secondY;
void myMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		clickCounter++;
		if (clickCounter > 2) {
			clickCounter = 1;
			glClear(GL_COLOR_BUFFER_BIT);
			vertices.clear();
			tessllation = 10;
			firstX = (2.0f * x) / (GLfloat)width - 1.0f;
			firstY = 1.0f - (2.0f * y) / (GLfloat)height;
			fprintf(stdout, "(%f,%f)\n", firstX, firstY);
		}
		else if (clickCounter == 2) {
			secondX = (2.0f * x) / (GLfloat)width - 1.0f;
			secondY = 1.0f - (2.0f * y) / (GLfloat)height;
			fprintf(stdout, "(%f,%f)\n", rx, ry);

			r = sqrt(pow(firstX - secondX, 2) + pow(firstY - secondY, 2));
			for (GLfloat theta = 0; theta <= PI * 2 + (PI / 2) / tessllation; theta = theta + (PI / 2) / tessllation) {
				vertices.push_back(r * cos(theta));
				vertices.push_back(r * sin(theta));
				vertices.push_back(0.0f);
			}
		}
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		tessllation+=10;
		vertices.clear();
		for (GLfloat theta = 0; theta <= PI * 2 + (PI / 2) / tessllation; theta = theta + (PI / 2) / tessllation) {
			vertices.push_back(r * cos(theta));
			vertices.push_back(r * sin(theta));
			vertices.push_back(0.0f);
		}
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	vtxPosition = glGetAttribLocation(programID, "vtxPosition");
	glVertexAttribPointer(vtxPosition, 3, GL_FLOAT, GL_FALSE, 0, ((void*)(0)));
	glEnableVertexAttribArray(vtxPosition);

}
int main(int argc, char** argv)
{
	//init GLUT and create Window
	//initialize the GLUT
	glutInit(&argc, argv);
	//GLUT_DOUBLE enables double buffering (drawing to a background buffer while the other buffer is displayed)
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	//These two functions are used to define the position and size of the window. 

	glutInitWindowPosition(300, 50);
	glutInitWindowSize(width, height);
	//This is used to define the name of the window.
	glutCreateWindow("Simple OpenGL Window");

	//call initization function
	init();

	programID = LoadShaders("VertexShader.txt", "FragmentShader.txt");
	glUseProgram(programID);

	glutMouseFunc(myMouse);
	glutDisplayFunc(renderScene);

	//enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}

void renderScene(void)
{
	//Clear all pixels
	glClear(GL_COLOR_BUFFER_BIT);
	//Let's draw something here
	if (vertices.size() >= 2) {
		glBindVertexArray(VAO);
		glVertexAttribPointer(vtxPosition, 3, GL_FLOAT, GL_FALSE, 0, ((void*)(0)));
		glEnableVertexAttribArray(vtxPosition);
		glDrawArrays(GL_LINE_STRIP, 0, vertices.size() / 3);
	}
	//Double buffer
	glutSwapBuffers();
}


void init()
{
	//initilize the glew and check the errors.
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s' \n", glewGetErrorString(res));
	}
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	//1. vertexBuffer 만들기 -> 검증은 glDrawArrays(GL_POINTS)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);


	//select the background color
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
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
