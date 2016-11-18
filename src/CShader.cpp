#include <CShader>

CShader::CShader()
{

}

CShader::CShader( char* fpath )
{

}

CShader::~CShader()
{

}
	
static int CShader::createFromTextfile( char* fpath )
{

	std::ifstream in("FileReadExample.cpp");
	std::string contents( (std::istreambuf_iterator<char>(in)), 
    					   std::istreambuf_iterator<char>() );

	char* sourceCode = contents.c_str();

	// @todo: hier weiterarbeiten ;) und fälle abhandeln
	// vertex_shader = glCreateShader(GL_VERTEX_SHADER);
 //    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
 //    glCompileShader(vertex_shader);




	return 0;
}
