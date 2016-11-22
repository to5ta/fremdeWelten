#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// #include <GL/glew.h>


/**
	CShader class provides functionality to easily create and compile shaders 
*/
class Shader
{
public:
	GLuint programID;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	
	void use();

};


