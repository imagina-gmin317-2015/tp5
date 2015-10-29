varying lowp vec4 col;
varying highp vec2 texCoord;
varying vec3 normal;

uniform sampler2D tex;
uniform vec3 lightDir;
void main() {
    /** Simple couleur */
    //gl_FragColor = col;

    /** Avec texture */
    //gl_FragColor = texture2D(tex,texCoord.st) * col;


    /** Affichage des normales */
    /*normal = normalize(normal);
    gl_FragColor = vec4(normal.x, normal.y, normal.z, 1);*/

    /** Affichage sable */
    /*if(gl_Color == vec4(0,0,0,1)){
        gl_FragColor = col;
    }else{
        gl_FragColor = gl_Color;
    }*/

    /** Toon shading */
    float intensity;
    intensity = dot(normalize(lightDir),normalize(normal));
    float factor = 1.0;

    if (intensity < 0.5)
        factor = 0.5;

    col *= vec4(factor, factor, factor, 1);
    gl_FragColor = col;
}
