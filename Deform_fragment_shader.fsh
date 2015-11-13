#version 130

uniform sampler2D texture1;

out vec4 color;

void main (void)
{

   vec4 objet = texture2D(texture1, vec2(gl_TexCoord[0]));
   gl_FragColor = objet;
}
