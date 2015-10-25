#include <QFile>
#include <QDebug>
#include <QVector>

#include "trianglewindow.h"
#include "tga.h"

static const char *geometryShaderSource =

        "void main()\n"
        "{\n"
        "   int i;\n"
        "   vec4 vertex;\n"
        "   for(i = 0; i < gl_VerticesIn; i++)\n"
        "   {\n"
        "       gl_Position = gl_PositionIn[i];\n"
        "       EmitVertex();\n"
        "   }\n"
        "   EndPrimitive();\n"

        "   for(i = 0; i < gl_VerticesIn; i++)\n"
        "   {\n"
        "       vertex = gl_PositionIn[i];\n"
        "       vertex.z = -vertex.z;\n"
        "       gl_Position = vertex;\n"
        "       EmitVertex();\n"
        "   }\n"
        "   EndPrimitive();\n"
        "}\n";

static const char *vertexShaderSource =
        "attribute highp vec4 posAttr;\n"
        "attribute lowp vec4 colAttr;\n"
        "attribute highp vec3 light_position;\n"
        "attribute lowp vec4 light_diffuse;\n"

        "attribute lowp float intensity;\n"

        "varying lowp vec4 col;\n"
        "varying lowp vec4 couleurDiffuse;\n"
        "varying highp vec3 light_p;\n"
        "varying lowp vec4 light_d;\n"
        "uniform highp mat4 matrix;\n"

        "float rand(vec2 co){\n"
        "   return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);\n"
        "}\n"

        "void main() {\n"
        "   gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;\n"

        "   col = colAttr;\n"
        "   light_p = light_position;\n"
        "   light_d = light_diffuse;\n"

        "   vec3 normals = gl_Normal;\n"
        "   //normals /= rand(posAttr.xy);\n"

        "   vec3 rayon = light_p - posAttr;\n"
        "   float angleLumiere = dot(normals, rayon);\n"
        "   couleurDiffuse = light_d * angleLumiere * intensity;\n"

        "   gl_Position = matrix * posAttr;\n"
        "}\n";

static const char *fragmentShaderSource =
        "varying lowp vec4 col;\n"
        "varying highp vec3 light_p;\n"
        "varying lowp vec4 light_d;\n"
        "varying lowp vec4 couleurDiffuse;\n"

        "uniform sampler2D tex;\n"
        "void main() {\n"

        "   gl_FragColor = texture2D(tex,gl_TexCoord[0].st) * col * couleurDiffuse;\n"
        "}\n";

GLuint textures[5]; // pour nos 5 textures
int rotY = 0;

TriangleWindow::TriangleWindow() : m_program(0), m_frame(0)
{
    texture = new Texture[5];
    intensity = 1.f;
}

GLuint TriangleWindow::loadShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    return shader;
}

void TriangleWindow::initialize()
{
    m_program = new QOpenGLShaderProgram(this);
    //m_program->addShaderFromSourceCode(QOpenGLShader::Geometry, geometryShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();

    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");

    m_light_position = m_program->attributeLocation("light_position");
    m_light_diffuse = m_program->attributeLocation("light_diffuse");

    m_intensity = m_program->attributeLocation("intensity");
    m_matrixUniform = m_program->uniformLocation("matrix");

    glEnable(GL_DEPTH_TEST);    // Active le Z-Buffer
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_TEXTURE_2D);

    /** Chargement des textures */
    texture[0].charger("./wall2.tga");
    texture[0].definit_bouclage(GL_REPEAT, GL_REPEAT);
    texture[0].definit_filtrage(GL_LINEAR, GL_LINEAR);
    texture[0].definit_melange(GL_MODULATE);

    texture[1].charger("./moisi.tga");
    texture[1].definit_bouclage(GL_REPEAT, GL_REPEAT);
    texture[1].definit_filtrage(GL_LINEAR, GL_LINEAR);
    texture[1].definit_melange(GL_MODULATE);

    texture[2].charger("./pierre.tga");
    texture[2].definit_bouclage(GL_REPEAT, GL_REPEAT);
    texture[2].definit_filtrage(GL_LINEAR, GL_LINEAR);
    texture[2].definit_melange(GL_MODULATE);

    texture[3].charger("./wall.tga");
    texture[3].definit_bouclage(GL_REPEAT, GL_REPEAT);
    texture[3].definit_filtrage(GL_LINEAR, GL_LINEAR);
    texture[3].definit_melange(GL_MODULATE);

    texture[4].charger("./grass.tga");
    texture[4].definit_bouclage(GL_REPEAT, GL_REPEAT);
    texture[4].definit_filtrage(GL_LINEAR, GL_LINEAR);
    texture[4].definit_melange(GL_MODULATE);
}

void TriangleWindow::DrawCurve(Point tabPointsOfCurve[], long nbPoints){
    glBegin(GL_LINE_STRIP);
    for(int i = 0 ; i < nbPoints ; i++){
        glVertex3d(tabPointsOfCurve[i].getX(), tabPointsOfCurve[i].getY(), tabPointsOfCurve[i].getZ());
    }
    glEnd();
}

void TriangleWindow::DrawSphere(float rayon, Point position, unsigned int m, unsigned int p){

    /*
x = rayon * sin (phi) * cos (theta)
y = rayon * sin (phi) * sin (theta)
z = rayon * cos (phi)
*/

    float phi, theta;
    Point** meridiens = new Point*[m];
    Point** paralleles = new Point*[p+1];
    for(uint i = 0 ; i < m ; i++){
        theta = (i * 2 * M_PI) / m;
        meridiens[i] = new Point[p+1];
        for(uint j = 0 ; j <= p ; j++){
            phi = (j * M_PI) / p;
            meridiens[i][j] = Point(position.getX() + rayon * sin (phi) * cos (theta), position.getY() + rayon * sin (phi) * sin (theta), position.getZ() + rayon * cos (phi));
        }
    }

    for(uint i = 0 ; i <= p ; i++){
        paralleles[i] = new Point[m+1];
        for(uint j = 0 ; j < m ; j++){
            paralleles[i][j] = meridiens[j][i];
        }
        paralleles[i][m] = paralleles[i][0];
    }

    for(uint i = 0 ; i < m; i++){
        DrawCurve(meridiens[i], p+1);
    }
    for(uint i = 0 ; i <= p; i++){
        DrawCurve(paralleles[i], m+1);
    }
}

void TriangleWindow::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -2);
    matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    m_program->setUniformValue(m_matrixUniform, matrix);
    m_program->setAttributeValue(m_colAttr, QVector4D(1,1,1,1));
    m_program->setAttributeValue(m_light_position, QVector3D(2,1,2));
    m_program->setAttributeValue(m_light_diffuse, QVector4D(0.3,0.3,0.3,1));
    m_program->setAttributeValue(m_intensity, intensity);

    //glLoadIdentity();
    rotY = 100.0f * m_frame / screen()->refreshRate();

    /** Triangle avec couleur
    glBegin(GL_TRIANGLES);
        glColor3f(1.f,0.f,0.f);
        glVertex3f( 0.0f, 0.707f ,0.f);
        glColor3f(0.f,1.f,0.f);
        glVertex3f( -0.5f, -0.5f ,0.f);
        glColor3f(0.f,0.f,1.f);
        glVertex3f( 0.5f, -0.5f ,0.f);
    glEnd();*/

    texture[1].utilise();
    DrawSphere(0.5f,Point(0,1,0), 50,50);

    glPushMatrix();

    glRotatef(rotY,0.0f,1.0f,0.0f);

    texture[0].utilise();
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.5f,1.f);
    glVertex3f( 0.0f, 0.707f ,0.f);
    glTexCoord2f(0.f,0.f);
    glVertex3f( -0.5f, -0.5f ,-0.5f);
    glTexCoord2f(1.f,0.f);
    glVertex3f( 0.5f, -0.5f ,-0.5f);
    glEnd();

    texture[1].utilise();
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.5f,1.f);
    glVertex3f( 0.0f, 0.707f ,0.f);
    glTexCoord2f(0.f,0.f);
    glVertex3f( -0.5f, -0.5f ,0.5f);
    glTexCoord2f(1.f,0.f);
    glVertex3f( 0.5f, -0.5f ,0.5f);
    glEnd();

    texture[2].utilise();
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.5f,1.f);
    glVertex3f( 0.0f, 0.707f ,0.f);
    glTexCoord2f(0.f,0.f);
    glVertex3f( -0.5f, -0.5f ,0.5f);
    glTexCoord2f(1.f,0.f);
    glVertex3f( -0.5f, -0.5f ,-0.5f);
    glEnd();

    texture[3].utilise();
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.5f,1.f);
    glVertex3f( 0.0f, 0.707f ,0.f);
    glTexCoord2f(0.f,0.f);
    glVertex3f( 0.5f, -0.5f ,-0.5f);
    glTexCoord2f(1.f,0.f);
    glVertex3f( 0.5f, -0.5f ,0.5f);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glRotatef(10,1.0f,0.0f,0.0f);

    texture[4].utilise();
    glBegin(GL_QUADS);
    glTexCoord2f(0.f,0.f);
    glVertex3f( -1.f, -1.f ,-1.f);
    glTexCoord2f(1.f,0.f);
    glVertex3f( 1.f, -1.f ,-1.f);
    glTexCoord2f(1.f,1.f);
    glVertex3f( 1.f, -1.f ,1.f);
    glTexCoord2f(0.f,1.f);
    glVertex3f( -1.f, -1.f ,1.f);
    glEnd();

    glPopMatrix();

    m_program->release();

    ++m_frame;
}

/**
 * @brief TriangleWindow::keyPressEvent, permet de gérer les évenements de type clavier.
 * @param event, évenement de type clavier
 */
void TriangleWindow::keyPressEvent(QKeyEvent *event)
{
    float gap = 0.1f;

    switch(event->key())
    {
    case '+':
        intensity+= gap;
        break;
    case '-':
        intensity-= gap;
        break;
    default:
        break;
    }
}

