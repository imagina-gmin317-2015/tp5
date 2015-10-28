varying lowp vec4 col;
varying highp vec2 texCoord;
varying vec3 normal;

uniform sampler2D tex;
void main() {
    gl_FragColor = texture2D(tex,texCoord) * col;
    //gl_FragColor = col;

    /*float intensity;
    vec4 color;
    vec3 lightDir = vec3(10,5,3);
    intensity = dot(normalize(lightDir),normalize(normal));

    if (intensity > 0.95)
        color = vec4(1,1,1,1.0);
    else if (intensity > 0.5)
        color = vec4(0.6,0.6,0.6,1.0);
    else if (intensity > 0.25)
        color = vec4(0.4,0.4,0.4,1.0);
    else
        color = vec4(0.2,0.2,0.2,1.0);

    gl_FragColor = color * col;*/

    float intensity;
    vec3 lightDir = vec3(10,5,3);
    intensity = dot(normalize(lightDir),normalize(normal));

    float factor = 1.0;
    if (intensity < 0.7)
        factor = 0.5;

    col *= vec4(factor, factor, factor, 1);

    gl_FragColor = col;
}
