#version 130

in vec3 posAttr;
in vect4 colAttr;
in vec3 normalAttr;
in vec2 texAttr;

out vec3 out_normal;
out vec2 out_texcoord;

uniform mat4x4 matrix;

void main()
{
	gl_Position=matrix*vec4(position,1.0);
	out_normal=normal; //Err too lazy to send view matrix
	out_texcoord=texcoord;
}
