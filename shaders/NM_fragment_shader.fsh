#version 130

in vec2 coordTexture;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 color;

varying vec3 v;
varying vec3 N;
varying mat3 normalMatrix;

void main(void)
{
    vec4 objet = texture2D(texture1, vec2(gl_TexCoord[0]));
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

    vec3 tmpVec = gl_LightSource[0].position.xyz - v;
    mat3 TBN = transpose(mat3(t,b,N));                                              // to tangent space

    vec3 lightVec;
    //lightVec = tmpVec*TBN;
    lightVec.x = dot(tmpVec, t);
    lightVec.y = dot(tmpVec, b);
    lightVec.z = dot(tmpVec, N);

    tmpVec = -tangent;
    vec3 eyeVec;
    eyeVec.x = dot(tmpVec, t);
    eyeVec.y = dot(tmpVec, b);
    eyeVec.z = dot(tmpVec, N);

    float distSqr = dot(lightVec, lightVec);
    float att = clamp(1.0 - 0.01 * sqrt(distSqr), 0.0, 1.0);
    vec3 lVec = lightVec * inversesqrt(distSqr);

    vec3 vVec = normalize(eyeVec);

    vec4 vAmbient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;

    float diffuse = max(dot(lVec, normal), 0.1 );

    vec4 vDiffuse = gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse *
                                    diffuse;

    float specular = pow(clamp(dot(reflect(-lVec, normal), vVec), 0.1, 1.0),
                     gl_FrontMaterial.shininess );

    vec4 vSpecular = gl_LightSource[0].specular * gl_FrontMaterial.specular *
                                     specular;

    gl_FragColor = (vAmbient*objet + vDiffuse*objet + vSpecular);//*att

    //gl_FragColor = objet;
}

