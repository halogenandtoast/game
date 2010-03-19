#include "vec3.h"

class Camera
{
public:
	vec3<float> pos, eye, up;
	Camera ();
	Camera (vec3<float> pc, vec3<float> ec, vec3<float> uc);
	~Camera();
	
	void set_look_at(float x, float y, float z);
	void set_position(float x, float y, float z);
	void update();
};
