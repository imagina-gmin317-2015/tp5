#version 130

uniform sampler2D texture1;

out vec4 color;

void main (void)
{
   vec4 objet = texture2D(texture1, vec2(gl_TexCoord[0]));
   gl_FragColor = objet;
}


/*
uniform sampler2D basetex;
uniform sampler2D heightMap;
varying vec3 eyeVec;

void main()
{

    float scale = 0.04;
    float bias = 0.02;
    float height = texture2D(heightMap, gl_TexCoord[0].st).r;
    //Our heightmap only has one color channel.
    float v = height * scale - bias;
    vec3 eye = normalize(eyeVec);
    vec2 newCoords = gl_TexCoord[0].st + (eye.xy * v);
    vec3 rgb = texture2D(basetex, newCoords).rgb;
    gl_FragColor = vec4(rgb, 1.0);
}

*/
