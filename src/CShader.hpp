#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <string>



/**
	CShader class provides functionality to easily create and compile shaders 
*/
class CShader
{
public:
	CShader();
	CShader( char* fpath );
	~CShader();
	
	static int createFromTextfile( char* fpath );

};


