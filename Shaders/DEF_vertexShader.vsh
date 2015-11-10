#version 130

in vec4 in_Vertex;

uniform mat4 matrix;


varying vec3 normal;

vec4 DoTwist(vec4 pos, float t )
{
    float st = sin(t);
    float ct = cos(t);
    vec4 new_pos;

    new_pos.x = pos.x*ct - pos.z*st;
    new_pos.z = pos.x*st + pos.z*ct;

    new_pos.y = pos.y;
    new_pos.w = pos.w;

    return new_pos;
}

void main(void)
{
    float ang ;
    if(gl_Vertex.y > gl_Vertex.z)
        ang = 45.0* 3.14159 / 180.0;
    else if (gl_Vertex.x > gl_Vertex.z)
        ang = 240.0* 3.14159 / 180.0;
    else
        ang = 300.0* 3.14159 / 180.0;

    vec4 changedPosition = DoTwist(gl_Vertex, ang);
    vec4 changedNormal = DoTwist(vec4(gl_Normal,ang), ang);

    gl_Position = matrix * changedPosition;
    normal = gl_NormalMatrix * changedNormal.xyz;

    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;


}


/*
//attribute vec3 tangent;
//attribute vec3 binormal;
uniform vec3 CAMERA_POSITION;
varying vec3 eyeVec;
void main()
{
    gl_Position = ftransform();
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    //mat3 TBNMatrix = mat3(tangent, binormal, gl_Normal);
    mat3 TBNMatrix = mat3(vec3(1,1,1), vec3(1,1,1), gl_Normal);
    eyeVec = CAMERA_POSITION - gl_Vertex.xyz;
    eyeVec *= TBNMatrix;
}

*/
