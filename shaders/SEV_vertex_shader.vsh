#version 130
in vec4 in_Vertex;
uniform mat4 matrix;

//in vec4 normal;
out float color_factor;
uniform vec4 light_direction;

in vec2 in_TexCoord0;
out vec2 coordTexture;
//out vec4 texture_color;
//uniform sampler2D texture2d;

void main(void)
{
    gl_Position = matrix * in_Vertex;

    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_TexCoord[1] = gl_TextureMatrix[1] * gl_MultiTexCoord0;

    //spherical environement mapping sur la texture2
    gl_TexCoord[1] = gl_MultiTexCoord0;
    vec3 u = normalize( vec3(matrix * in_Vertex) );
    vec3 n = normalize( vec3(matrix) );
    vec3 r = reflect( u, n );
    float m = 2.0 * sqrt( r.x*r.x + r.y*r.y + (r.z+1.0)*(r.z+1.0) );
    gl_TexCoord[1].s = r.x/m + 0.5;
    gl_TexCoord[1].t = r.y/m + 0.5;
}
