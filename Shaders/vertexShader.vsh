#version 130
in vec4 Vertex;
uniform mat4 matrix;

varying vec3 N;
varying vec3 v;

void main(void){

    gl_Position = matrix * gl_Vertex;

    v = vec3(gl_ModelViewMatrix * gl_Vertex);
    N = normalize(gl_NormalMatrix * gl_Normal);

    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
}
