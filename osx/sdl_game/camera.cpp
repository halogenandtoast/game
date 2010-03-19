#include "camera.h"
#include "SDL.h"
#include "SDL_opengl.h"



Camera::Camera() {
	pos = vec3<float>(0.0, 0.0, 0.0);
	eye = vec3<float>(0.0, 0.0, -5.0);
	up = vec3<float>(0.0, 1.0, 0.0);
}

Camera::~Camera() {
}

void Camera::update() {
	gluLookAt(pos.x,pos.y,pos.z,eye.x,eye.y,eye.z,up.x,up.y,up.z);
}


void Camera::set_look_at(float x, float y, float z) {
	eye = vec3<float>(x, y, z);
}

void Camera::set_position(float x, float y, float z) {
	float diffz = z - pos.z;
	float diffx = x - pos.x;
	float diffy = y - pos.y;
	pos = vec3<float>(x, y, z);
	set_look_at(eye.x + diffx, eye.y + diffy, eye.z + diffz);
}