#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "SDL.h"
#include "SDL_opengl.h"

class HeightMap {
public:
	GLuint id;
	GLfloat *map;
	SDL_Surface *surface;
	int width, height;
	void load(char *filename);
	~HeightMap();
	Uint32 GetPixel(int x, int y);
	void render();
};

#endif