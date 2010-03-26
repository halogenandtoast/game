#include "light.h"

static int lights = 0;

void Light::setDiffuse(Color c) {
	diffuse = c;
}

void Light::setAmbient(Color c) {
	ambient = c;
}

void Light::setSpecular(Color c) {
	specular = c;
}

void Light::setPosition(vec3<float> pos) {
	position = pos;
}

void Light::create() {
	float pos[4] = {position.x, position.y, position.z, 1.0};
	float amb[4] = {ambient.r, ambient.g, ambient.b, ambient.a};
	float dif[4] = {diffuse.r, diffuse.g, diffuse.b, diffuse.a};
	
	glLightfv(GL_LIGHT0 + lights, GL_POSITION, pos);
	glLightfv(GL_LIGHT0 + lights, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0 + lights, GL_DIFFUSE, dif);
	
	
	glLightf(GL_LIGHT0 + lights, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT0 + lights, GL_LINEAR_ATTENUATION, 0.0);
	glLightf(GL_LIGHT0 + lights, GL_QUADRATIC_ATTENUATION, 0.0);
	
	lights++;
}