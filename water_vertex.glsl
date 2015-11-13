uniform float waterHeight;
uniform float time;
varying vec3 N;
varying vec3 V;
varying vec4 pos;

void main(void)
{
  vec4 p = vec4(gl_Vertex.xy, waterHeight + sin(gl_Vertex.x * 100 + time) * 0.005 + sin(gl_Vertex.y * 100 + time) * 0.005, gl_Vertex.w);
  vec3 normal = normalize(vec3(-cos(gl_Vertex.x * 100 + time), -cos(gl_Vertex.y * 100 + time), 1));

  gl_Position = gl_ModelViewProjectionMatrix * p;
  
  V = vec3(gl_ModelViewProjectionMatrix * p);       
  N = normalize(gl_NormalMatrix * normal);

  pos = gl_Vertex;
}
