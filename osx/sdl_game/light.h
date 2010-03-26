#ifndef __LIGHT__
#define __LIGHT__

#include "SDL.h"
#include "SDL_opengl.h"
#include "vec3.h"
#include "color.h"

class Light
{
public:
	vec3<float> position;
	Color diffuse, ambient, specular;
	Light () : position(0.0, 0.0, 0.0) {};
	Light (vec3<float> p) : position(p) {};
	
	void setDiffuse(Color c);
	void setAmbient(Color c);
	void setSpecular(Color c);
	void setPosition(vec3<float> pos);
	void create();
	
};

#endif