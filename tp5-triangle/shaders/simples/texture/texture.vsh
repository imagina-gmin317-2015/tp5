attribute highp vec4 posAttr;
attribute lowp vec4 colAttr;
attribute mediump vec4 texAttr;

varying lowp vec4 col;
varying mediump vec4 texc;

uniform highp mat4 matrix;
//Interface block only after 150
void main() {
  // col = gl_FrontColor; black or default color for front face color
  //col = gl_Color; // Default color used with glColor3f()
   col = colAttr;
   texc = texAttr;

   gl_Position = matrix * posAttr;
}
