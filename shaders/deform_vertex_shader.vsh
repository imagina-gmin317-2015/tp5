#version 130
in vec4 in_Vertex;
uniform mat4 matrix;

varying vec3 N;
varying vec3 v;
varying mat3 normalMatrix;

void main()
{
    if(in_Vertex.x < 0.3)
        gl_Position = matrix * vec4(in_Vertex.x+0.25, in_Vertex.y, in_Vertex.z, 1);
    else
        gl_Position = matrix * in_Vertex;

    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
}
