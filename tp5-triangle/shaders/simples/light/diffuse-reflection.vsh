attribute highp vec4 posAttr;
attribute lowp vec4 colAttr;

varying lowp vec4 col;
//varying vec3 N;
//varying vec3 v;

uniform highp mat4 matrix;

void main(void)
{

   //v = vec3(gl_ModelViewMatrix * gl_Vertex);
   //N = normalize(gl_NormalMatrix * gl_Normal);
   col = colAttr;
   //gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   gl_Position = matrix * posAttr;
}
