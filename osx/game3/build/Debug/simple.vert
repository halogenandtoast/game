varying vec3 normal;
varying vec3 pointPos;
varying vec3 tricol;

void main()
{
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	pointPos = vec3(gl_ModelViewMatrix * gl_Vertex); 
	normal = normalize(gl_NormalMatrix * gl_Normal);
}