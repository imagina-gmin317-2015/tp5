#ifndef TRIANGLETP5
#define TRIANGLETP5

#include "texture.h"
#include "ply.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QtGui/QOpenGLTexture>

#include <QtCore/qmath.h>
#include <QGLWidget>
#include <QFile>

///Diffuse
static const char *vertexShaderSourceDiff =
        "varying vec3 N;\n"
        "varying vec3 v;\n"

        "void main(void)\n"
        "{\n"
           "v = vec3(gl_ModelViewMatrix * gl_Vertex);\n"
           "N = normalize(gl_NormalMatrix * gl_Normal);\n"

           "gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
        "}\n";

static const char *fragmentShaderSourceDiff =
        "varying vec3 N;\n"
        "varying vec3 v;\n"
        "uniform sampler2D colorMap;\n"

        "void main(void)\n"
        "{\n"
           "vec3 L = normalize(gl_LightSource[0].position.xyz - v);\n"
           "vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);\n"
           "Idiff = clamp(Idiff, 0.0, 1.0);\n"
           "vec4 color = texture2D( colorMap, gl_TexCoord[0].st);\n"
           "gl_FragColor = Idiff;\n"
        "}\n";

/// FIN Diffuse


///Spherical environnement mapping
static const char *vertexShaderSourceSEM =
    "void main() {\n"
        "gl_Position = ftransform();\n"
        "gl_TexCoord[0] = gl_MultiTexCoord0;\n"
        "vec3 u = normalize( vec3(gl_ModelViewMatrix * gl_Vertex) );\n"
        "vec3 n = normalize( gl_NormalMatrix * gl_Normal );\n"
        "vec3 r = reflect( u, n );\n"
        "float m = 2.0 * sqrt( r.x*r.x + r.y*r.y + (r.z+1.0)*(r.z+1.0) );\n"
        "gl_TexCoord[1].s = r.x/m + 0.5;\n"
        "gl_TexCoord[1].t = r.y/m + 0.5;\n"
     "}\n";

static const char *fragmentShaderSourceSEM =
    "uniform sampler2D colorMap;\n"
    "uniform sampler2D envMap;\n"
    "void main (void)\n"
    "{\n"
        "vec4 color = texture2D( colorMap, gl_TexCoord[0].st);\n"
        "vec4 env = texture2D( envMap, gl_TexCoord[1].st);\n"
        "gl_FragColor = color + env*0.4;\n"
    "}\n";

///FIN Spherical environnement mapping


///Normal Mapping
static const char *vertexShaderSourceNorMap =
        "attribute vec3 position;\n"
        "attribute vec3 normal;\n"
        "attribute vec2 texCoords;\n"
        "attribute vec3 tangent;\n"
        "attribute vec3 bitangent;\n"

        "uniform mat4 projection;\n"
        "uniform mat4 view;\n"
        "uniform mat4 model;\n"

        "uniform vec3 lightPos;\n"
        "uniform vec3 viewPos;\n"

        "void main()\n"
        "{\n"
            "gl_Position = projection * view * model * vec4(position, 1.0f);\n"
            "fragPos = vec3(model * vec4(position, 1.0)); \n"
            "texCoords = texCoords;\n"

            "mat3 normalMatrix = transpose(inverse(mat3(model)));\n"
            "vec3 T = normalize(normalMatrix * tangent);\n"
            "vec3 B = normalize(normalMatrix * bitangent);\n"
            "vec3 N = normalize(normalMatrix * normal);\n"

            "mat3 TBN = transpose(mat3(T, B, N));  \n"
            "tangentLightPos = TBN * lightPos;\n"
            "tangentViewPos  = TBN * viewPos;\n"
            "tangentFragPos  = TBN * fragPos;\n"
        "}\n";




static const char *fragmentShaderSourceNorMap =        
        "out vec4 FragColor;\n"

        "in VS_OUT {\n"
            "vec3 FragPos;\n"
            "vec2 TexCoords;\n"
            "vec3 TangentLightPos;\n"
            "vec3 TangentViewPos;\n"
            "vec3 TangentFragPos;\n"
        "} fs_in;\n"

        "uniform sampler2D diffuseMap;\n"
        "uniform sampler2D normalMap;\n"

        "uniform bool normalMapping;\n"

        "void main()\n"
        "{\n"
            // Obtain normal from normal map in range [0,1]
            "vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;\n"
            // Transform normal vector to range [-1,1]
            "normal = normalize(normal * 2.0 - 1.0);\n"  // this normal is in tangent space

            // Get diffuse color
            "vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;\n"
            // Ambient
            "vec3 ambient = 0.1 * color;\n"
            // Diffuse
            "vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);\n"
            "float diff = max(dot(lightDir, normal), 0.0);\n"
            "vec3 diffuse = diff * color;\n"
            // Specular
            "vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);\n"
            "vec3 reflectDir = reflect(-lightDir, normal);\n"
            "vec3 halfwayDir = normalize(lightDir + viewDir);\n"
            "float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);\n"
            "vec3 specular = vec3(0.2) * spec;\n"

            "FragColor = vec4(ambient + diffuse + specular, 1.0f);\n"
        "}\n";
/// FIN Normal Mapping

/// Déformation Géométrique
static const char *vertexShaderSourceGeo =
        "varying vec3 N;\n"
        "varying vec3 v;\n"

        "void main(void)\n"
        "{\n"
           "v = vec3(gl_ModelViewMatrix * gl_Vertex);\n"
           "N = normalize(gl_NormalMatrix * gl_Normal);\n"

           "int v2 = rand() % 100 + 1;"

           "gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex * v2;\n"
        "}\n";

static const char *fragmentShaderSourceGeo = //Comme pour diffuse en fait
        "varying vec3 N;\n"
        "varying vec3 v;\n"

        "void main(void)\n"
        "{\n"
           "vec3 L = normalize(gl_LightSource[0].position.xyz - v);\n"
           "vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);\n"
           "Idiff = clamp(Idiff, 0.0, 1.0);\n"

          "gl_FragColor = Idiff;\n"
        "}\n";

///FIN Déformation géométrique


class TriangleTP5 : public OpenGLWindow
{
public:
    TriangleTP5(int);//choix shader 1 pour diffuse, 2 pour SEM

    void initialize() Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;

    void displayTextureColor();
    void displayTexture2D();
    void initTexture2D(int,QString);
    void displaySphere();


private:
    GLuint loadShader(GLenum type, const char *source);

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;
    int m_frame;

    //Texture
    //GLuint idTexture;
    //QImage buffTexture;
    QImage textureTE;

    int choixShader;

    Ply* sphere;
    Texture** tex;

};



#endif // TRIANGLETP5

