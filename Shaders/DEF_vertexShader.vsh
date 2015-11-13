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
uniform sampler2D texture2;

uniform mat4 matrix;
varying vec3 eyeVec;
varying vec3 N;
varying mat3 normalMatrix;

void main()
{
    vec4 NormalMap = texture2D(texture2, vec2(gl_TexCoord[1]));

    //convert to range -1.0 to 1.0
    vec3 normal = normalize(NormalMap.rgb * 2.0 - 1.0);

    vec3 tangent;
    vec3 binormal;

    vec3 c1 = cross(normal, vec3(0.0, 0.0, 1.0));
    vec3 c2 = cross(normal, vec3(0.0, 1.0, 0.0));

    if(length(c1)>length(c2))
    {
        tangent = c1;
    }
    else
    {
        tangent = c2;
    }

    tangent = normalize(tangent);

    binormal = cross(normal, tangent);
    binormal = normalize(binormal);

    vec3 t = normalize(normalMatrix * tangent);
    vec3 b = cross(N, t);

    gl_Position = ftransform() * matrix;
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    //mat3 TBNMatrix = mat3(tangent, binormal, gl_Normal);
    mat3 TBNMatrix = mat3(t, b, gl_Normal);
    eyeVec = CAMERA_POSITION - gl_Vertex.xyz;
    eyeVec *= TBNMatrix;
}

*/


