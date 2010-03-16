varying vec3 normal;
varying vec3 pointPos;
varying vec3 lightDir;

void main()
{	
	vec3 lightPos = vec3(0.0,0.0,-3.0);
	vec3 lightDir = normalize(vec3(1,1,1));
	
	float s = 20.0;
	vec3 illumDir = normalize(lightPos-pointPos);
	float diffuse = dot(normal, illumDir);
	vec3 tricol = diffuse * vec3(1,0.5,0.3);
	gl_FragColor = vec4(pow(dot(reflect(illumDir, normal), lightDir), s) * tricol, 1.0) + vec4(tricol, 1.0);
}