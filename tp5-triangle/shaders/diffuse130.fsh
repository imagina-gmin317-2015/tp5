#version 130
in  vec3 out_normal;
out vec4 out_color;
const vec3 lightDir=vec3(-1,1,0);
void main()
{
	vec3 nlightDir=normalize(lightDir);
	float diffuse=clamp(dot(nlightDir,out_normal),0,1);

	out_color=vec4(diffuse*vec3(1,1,1),1.0);
}
