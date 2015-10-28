attribute highp vec4 posAttr;
attribute lowp vec4 colAttr;
attribute highp vec2 texAttr;
attribute highp vec3 normalAttr;

varying lowp vec4 col;
varying highp vec2 texCoord;
varying vec3 normal;

uniform highp mat4 matrix;

void main() {
    normal = normalAttr;

    col = colAttr;
    texCoord = texAttr;
    gl_Position = matrix * posAttr;
}
