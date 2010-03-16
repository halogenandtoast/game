#ifndef GLSL_H_
#define GLSL_H_

#include <cstdio>
#include <iostream>
using std::cout;
using std::endl;
#include "GLUT/glut.h"

/**** GLSL Helper functions ****/

// Create GLSL program
GLuint createGLSLProgram(char *vs_file, char *fs_file);

// Cleaup GLSL program and shaders
void cleanupGLSL(GLuint prog_id);

// Create a shader object from file
GLuint createShaderFromFile(char *filename, GLuint shader_type);

// Shader creation error reporting
bool reportShaderError(GLuint shader);
// Program creation error reporting
bool reportProgramError(GLuint prog_id);


#endif