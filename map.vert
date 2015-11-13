attribute vec4 posAttr;
attribute vec4 colAttr;
attribute vec4 normal;
varying float col;
varying vec4 color;
uniform mat4 matrix;
uniform vec4 light_direction;

void main(void)
{
    color = colAttr;
    col = max(dot(normal, light_direction), 0.0);
    gl_Position = matrix * posAttr;
}
