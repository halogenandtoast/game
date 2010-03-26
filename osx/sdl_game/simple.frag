varying vec3 normal;
varying vec3 pointPos;
varying vec3 lightDir;

void main()
{	
	vec3 L = normalize(gl_LightSource[0].position.xyz - pointPos);
	vec3 E = normalize(-pointPos);
	vec3 R = normalize(-reflect(L, normal));
	
	vec4 Iamb = gl_FrontLightProduct[0].ambient;
	
	vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(normal, L), 0.0);
	Idiff = clamp(Idiff, 0.0, 1.0);
	
	vec4 Ispec = gl_FrontLightProduct[0].specular * pow(max(dot(R, E), 0.0), 0.3 * gl_FrontMaterial.shininess);
	Ispec = clamp(Ispec, 0.0, 1.0);
	
	gl_FragColor = gl_FrontLightModelProduct.sceneColor + Iamb + Idiff + Ispec;
}