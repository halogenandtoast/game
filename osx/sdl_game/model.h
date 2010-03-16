#ifndef MODEL_H
#define MODEL_H

#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "SDL.h"
#include "SDL_opengl.h"

struct myVertex
{
	float x,y,z;
	float nx,ny,nz;
};

bool loadModel(char *filename, myVertex* &vertices, int &num_verts);
#define NUM_BUFFERS 1
class Model {
public:
	GLuint buffer_name;
	myVertex *vertices;
	int num_verts;
	
	~Model();
	
	bool loadModel(char *filename);
	void render();
};

#endif