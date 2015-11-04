uniform float waterHeight;
uniform float time;
varying vec3 N;
varying vec3 V;

void main(void)
{
  vec4 pos = vec4(gl_Vertex.xy, waterHeight + sin(gl_Vertex.x * 100 + time) * 0.004 + sin(gl_Vertex.y * 100 + time) * 0.004, gl_Vertex.w);
  vec3 normal = normalize(vec3(-cos(gl_Vertex.x * 100 + time), -cos(gl_Vertex.y * 100 + time), 1));

  gl_Position = gl_ModelViewProjectionMatrix * pos;
  
  V = vec3(gl_ModelViewProjectionMatrix * pos);       
  N = normalize(gl_NormalMatrix * normal);
}
