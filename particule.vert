attribute vec4 posAttr;
uniform mat4 matrix;
uniform float size;

void main(void)
{
    gl_PointSize = size;
    gl_Position = matrix * posAttr;
}
