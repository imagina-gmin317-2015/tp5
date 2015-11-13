varying float col;
varying vec4 color;
uniform vec4 ambiant_color;

void main(void)
{
    gl_FragColor = color * (ambiant_color * col);
}
