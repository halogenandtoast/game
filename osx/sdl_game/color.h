#ifndef __COLOR__
#define __COLOR__

class Color
{
public:
	float r, g, b, a;
	Color () : r(0.0), g(0.0), b(0.0), a(1.0) {};
	Color (float rc, float gc, float bc) : r(rc), g(gc), b(bc), a(1.0) {};
	Color (float rc, float gc, float bc, float ac) : r(rc), g(gc), b(bc), a(ac) {};
	
	//vec3 operator = (const vec3 &v) { x = v.x; y = v.y; z = v.z; return *this; }
//	vec3 operator - () const { return vec3(-x, -y, -z); }
//	vec3 operator + (const vec3 &v) const { return vec3(x + v.x, y + v.y, z + v.z); }
//	vec3 operator - (const vec3 &v) const { return vec3(x - v.x, y - v.y, z - v.z); }
//	
//	const vec3 &operator += (const vec3 &v) { *this = *this + v; return *this; }
//	const vec3 &operator -= (const vec3 &v) { *this = *this - v; return *this; }
//	
//	template<typename S> const vec3 &operator *= (S f) { *this = *this * f; return *this; }
//	template<typename S> const vec3 &operator /= (S f) { *this = *this / f; return *this; }
//	
//	float operator * (const vec3 &v) const { return x * v.x + y * v.y + z * v.z; }
//	
//	vec3 operator ^ (const vec3 &v) const
//	{
//		vec3 r;
//		r.x = y * v.z - z * v.y;
//		r.y = z * v.x - x * v.z;
//		r.z = x * v.y - y * v.x;
//		return r;
//	}
//	
//	const vec3 &operator ^= (const vec3 &v) { *this = *this ^ v; return *this; }
//	
//	T length() const { return sqrt(x * x + y * y + z * z); }
//	vec3 normalize() const { T l = 1/length(); return vec3(x * l, y * l, z * l); }
//	T distance (const vec3 &v) const { return (v - *this).length(); }	
	
};

#endif