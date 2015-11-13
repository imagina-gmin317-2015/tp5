#version 130

in vec2 coordTexture;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 color;


void main(void)
{
    vec4 objet = texture2D(texture1, vec2(gl_TexCoord[0]));
    vec4 reflet = texture2D(texture2, vec2(gl_TexCoord[1]));
    gl_FragColor = objet + reflet*0.4;
}
