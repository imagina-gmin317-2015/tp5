#version 130

in vec4 in_V;
uniform mat4 matrix;

in vec2 in_TexCoord0;


void main(void)
{
    gl_Position = matrix * in_V;

    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_TexCoord[1] = gl_TextureMatrix[1] * gl_MultiTexCoord0;

    gl_TexCoord[1] = gl_MultiTexCoord0;
    vec3 u = normalize(vec3(matrix * in_V));
    vec3 n = normalize(vec3(matrix));
    vec3 r = reflect(u, n);

    float m = 2.0 * sqrt(r.x*r.x + r.y*r.y + (r.z+1.0)*(r.z+1.0));

    gl_TexCoord[1].s = r.x/m + 0.5;
    gl_TexCoord[1].t = r.y/m + 0.5;
}
