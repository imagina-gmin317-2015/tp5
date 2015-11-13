varying vec3 N;
varying vec3 v;
//varying vec4 texc;
//varying vec4 col;
uniform sampler2D texture;

void main(void)
{
  //Get Vector between light source and vector v (surely to compute L in VertexShader)
   vec3 L = normalize(gl_LightSource[0].position.xyz - v);
   //Get Light0 diffuse properties
   vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);
   Idiff = clamp(Idiff, 0.0, 1.0);
   //vec4 texCol = texture2D(texture,gl_TexCoord[0].st);
   //gl_FragColor = texCol * Idiff;
   //vec4 texCol = texture2D(texture,texc.st) * col;
   //gl_FragColor = Idiff * texCol;
   gl_FragColor = Idiff;
}
