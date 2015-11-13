attribute highp vec4 qt_Vertex;
attribute highp vec4 qt_MultiTexCoord0;
uniform highp mat4 qt_ModelViewProjectionMatrix;
varying highp vec4 qt_TexCoord0;

varying vec3 Normal;

void main(void)
{

// Front color
gl_FrontColor = gl_Color;

// Determine the normal of the vertex
Normal = gl_NormalMatrix * gl_Normal;

// Textures coordinates
qt_TexCoord0[0] = qt_MultiTexCoord0;

// The position of the vertex
gl_Position = ftransform();

}
