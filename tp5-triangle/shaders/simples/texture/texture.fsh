uniform sampler2D texture;
varying mediump vec4 texc;
varying lowp vec4 col;
void main() {
 gl_FragColor = texture2D(texture,texc.st) * col;
// gl_FragColor = gl_Color;//Work but hideous not except color
//Ignore color of texture vertex
//gl_FragColor = col;
//Just texture color
//   gl_FragColor = texture2D(texture,texc.st);
}
