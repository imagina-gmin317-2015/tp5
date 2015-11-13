#version 130
in vec4 in_Vertex;
uniform mat4 matrix;

void main(void)
{
    vec4 V;
    V.x = gl_Vertex.x;
    V.y = gl_Vertex.y;
    V.w = gl_Vertex.w;

    if(gl_Vertex.z > 0.5)
    {
        V.z -= 0.5;
    }
    else
        V.z = gl_Vertex.z;

    gl_Position = matrix * V;


    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
}
