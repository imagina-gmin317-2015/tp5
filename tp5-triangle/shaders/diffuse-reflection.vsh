varying vec3 N;
varying vec3 v;

attribute highp vec4 posAttr;
attribute highp vec3 normalAttr;

uniform highp mat4 matrix;

void main(void)
{

   //v = vec3(gl_ModelViewMatrix * gl_Vertex);
   //N = normalize(gl_NormalMatrix * gl_Normal);
    N = normalize(normalAttr);
    v = vec3(matrix * posAttr);
   gl_Position = matrix * posAttr;

}
