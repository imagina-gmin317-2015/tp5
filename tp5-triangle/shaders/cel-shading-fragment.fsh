uniform sampler2D qt_Texture0;
varying highp vec4 qt_TexCoord0;

void main(void)
uniform sampler2D Texture0;

varying vec3 Normal;
void main (void)
{

vec4 color = texture2D( Texture0 , vec2( qt_TexCoord[0] ) );
//TODO
gl_FragColor = color;
}
