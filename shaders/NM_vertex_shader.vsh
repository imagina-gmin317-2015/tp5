#version 130
in vec4 in_Vertex;
uniform mat4 matrix;

varying vec3 N;
varying vec3 v;
varying mat3 normalMatrix;

void main()
{
    gl_Position = matrix * in_Vertex;

    N = normalize(gl_NormalMatrix * gl_Normal);
    v = vec3(matrix * in_Vertex);
    normalMatrix = gl_NormalMatrix;

    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_TexCoord[1] = gl_TextureMatrix[1] * gl_MultiTexCoord0;
}
