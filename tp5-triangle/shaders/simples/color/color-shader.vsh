attribute highp vec4 posAttr;
attribute lowp vec4 colAttr;

varying lowp vec4 col;

uniform highp mat4 matrix;
void main() {
   //setup color for fragment (surely exists a built-in gl_color)
   col = gl_Color;
   //setup position
   gl_Position = matrix * posAttr;
}
