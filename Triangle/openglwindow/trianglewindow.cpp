#include <QFile>
#include <QDebug>

#include "trianglewindow.h"
#include "tga.h"

static const char *vertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "varying lowp vec4 col;\n"
    "uniform highp mat4 matrix;\n"
    "void main() {\n"
    "   col = colAttr;\n"
    "   gl_Position = matrix * posAttr;\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = col;\n"
    "}\n";

GLuint textures[4]; // pour nos 4 textures
int rotY = 0;

TriangleWindow::TriangleWindow() : m_program(0), m_frame(0)
{}

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
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");

    glEnable(GL_DEPTH_TEST);    // Active le Z-Buffer
    glDepthFunc(GL_LEQUAL);
}

void TriangleWindow::loadTexture(char* str, unsigned int t){
    byte *img;
    int width, height, bpp;
    img = load_tga(str, &width, &height, &bpp);

    if( img != NULL )
    {
        glBindTexture(GL_TEXTURE_2D, textures[t]);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height,0, GL_RGB, GL_UNSIGNED_BYTE, img);
        delete[] img;
    }else{
        qDebug() << "Impossible de charger l'image.";
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_TEXTURE_2D);
}

void TriangleWindow::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
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

    glPushMatrix();

    glRotatef(rotY,0.0f,1.0f,0.0f);

    loadTexture("./wall2.tga", 0);
    glBegin(GL_TRIANGLES);
        glTexCoord2f(0.5f,1.f);
        glVertex3f( 0.0f, 0.707f ,0.f);
        glTexCoord2f(0.f,0.f);
        glVertex3f( -0.5f, -0.5f ,-0.5f);
        glTexCoord2f(1.f,0.f);
        glVertex3f( 0.5f, -0.5f ,-0.5f);
    glEnd();

    loadTexture("./moisi.tga", 0);
    glBegin(GL_TRIANGLES);
        glTexCoord2f(0.5f,1.f);
        glVertex3f( 0.0f, 0.707f ,0.f);
        glTexCoord2f(0.f,0.f);
        glVertex3f( -0.5f, -0.5f ,0.5f);
        glTexCoord2f(1.f,0.f);
        glVertex3f( 0.5f, -0.5f ,0.5f);
    glEnd();

    loadTexture("./pierre.tga", 0);
    glBegin(GL_TRIANGLES);
        glTexCoord2f(0.5f,1.f);
        glVertex3f( 0.0f, 0.707f ,0.f);
        glTexCoord2f(0.f,0.f);
        glVertex3f( -0.5f, -0.5f ,0.5f);
        glTexCoord2f(1.f,0.f);
        glVertex3f( -0.5f, -0.5f ,-0.5f);
    glEnd();

    loadTexture("./wall.tga", 0);
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

    loadTexture("./grass.tga", 0);
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

    ++m_frame;
}

