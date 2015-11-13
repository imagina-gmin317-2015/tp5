#version 130
in vec4 in_V;
uniform mat4 matrix;

varying vec3 normale;
varying vec3 vertex;
varying mat3 normale_Matrix;

void main()
{
    gl_Position = matrix * in_V;

    normale = normalize(gl_NormalMatrix * gl_Normal);
    vertex = vec3(matrix * in_V);

    normale_Matrix = gl_NormalMatrix;

    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_TexCoord[1] = gl_TextureMatrix[1] * gl_MultiTexCoord0;
}
