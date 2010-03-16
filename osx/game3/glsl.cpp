#include "glsl.h"

// Globals
GLuint	vs_id;		// vertex shader object id
GLuint	fs_id;		// fragment shader object id


// Create GLSL program
GLuint createGLSLProgram(char *vs_file, char *fs_file)
{
	// create vertex and fragment shader objects
	std::cout << "\n\nVertex Shader:" << std::endl;
	vs_id = createShaderFromFile(vs_file, GL_VERTEX_SHADER);
	std::cout << "Fragment Shader:" << std::endl;
	fs_id = createShaderFromFile(fs_file, GL_FRAGMENT_SHADER);
	
	// create program object
	GLuint prog_id = glCreateProgram();
	
	// attach shaders to program
	glAttachShader(prog_id, vs_id);
	glAttachShader(prog_id, fs_id);
	
	// link program
	glLinkProgram(prog_id);
	if(!reportProgramError(prog_id)) return 0;
	return prog_id;
}

// Cleaup GLSL program and shaders
void cleanupGLSL(GLuint prog_id)
{
	// detatch shaders
	glDetachShader(prog_id, vs_id);
	glDetachShader(prog_id, fs_id);
	glDeleteShader(vs_id);
	glDeleteShader(fs_id);
	glDeleteProgram(prog_id);
}

// Create shader object from file
GLuint createShaderFromFile(char *filename, GLuint shader_type)
{
	// read file into a string
	FILE *fp=NULL;
	int  fileSize;
	char *shader_source=NULL;
	
	// open file
	fp = fopen(filename,"rb");
	if(fp==NULL)
	{
		std::cout << "Could not open file" << std::endl;
		return 0;
	}
	
	// get file size
	fseek(fp,0,SEEK_END);
	fileSize = ftell(fp);
	rewind(fp);
	
	// allocate space for shader source
	shader_source = new char[fileSize+1];
	if(shader_source==NULL)
	{
		std::cout << "Could not allocate space for shader source" << std::endl;
		fclose(fp);
		return 0;
	}
	
	// read file
	if(fread(shader_source,1,fileSize,fp)!=fileSize)
	{
		std::cout << "Could not read file" << std::endl;
		fclose(fp);
		return 0;
	}
	fclose(fp);
	shader_source[fileSize] = '\0';
	
	// print out the shader source
	std::cout << "------------------------" << std::endl;
	std::cout << "Shader source code:" << std::endl;
	std::cout << "------------------------" << std::endl;
	std::cout << shader_source << std::endl;
	std::cout << "------------------------\n" << std::endl;
	
	
	// Create shader
	GLuint vs = glCreateShader(shader_type);
	glShaderSource(vs, 1, (const GLchar **)&shader_source, NULL);
	delete [] shader_source;
	glCompileShader(vs);
	
	if(!reportShaderError(vs)) return 0;
	return vs;
}


// Shader creation error reporting
bool reportShaderError(GLuint shader)
{
	int _status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &_status);
	if(_status==GL_TRUE) return true;
	else
	{
		std::cout << "** Compile Errors **" << std::endl;
		char error_log[4096];
		int error_length;
		glGetShaderInfoLog(shader, 4096, &error_length, error_log);
		std::cout << error_log << "\n" << std::endl;
		return false;
	}
}

// Program creation error reporting
bool reportProgramError(GLuint prog_id)
{
	int _status;
	glGetProgramiv(prog_id, GL_LINK_STATUS, &_status);
	if(_status==GL_TRUE) return true;
	else
	{
		std::cout << "** Compile Errors **" << std::endl;
		char error_log[4096];
		int error_length;
		glGetProgramInfoLog(prog_id, 4096, &error_length, error_log);
		std::cout << error_log << "\n" << std::endl;
		return false;
	}
}