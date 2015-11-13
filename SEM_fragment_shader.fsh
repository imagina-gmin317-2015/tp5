#version 130

uniform sampler2D texture1;
uniform sampler2D texture2;

void main(void)
{
    vec4 obj = texture2D(texture1, vec2(gl_TexCoord[0]));
    vec4 r = texture2D(texture2, vec2(gl_TexCoord[1]));
    gl_FragColor = obj + r*0.6;
}
