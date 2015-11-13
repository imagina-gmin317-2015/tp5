//#include "openglwindow.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QtOpenGL/QGLWidget>
#include <QtCore/qmath.h>
#include <point.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "gamewindow.h"
#include "gamewindowserver.h"
#include "gamewindowclient.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <time.h>
#include <sys/time.h>
#include <iostream>

#include <QtCore>
#include <QtGui>
#include <QThread>

#include "mythread.h"

using namespace std;

#define PRINTEMPS 0
#define ETE 1
#define AUTOMNE 2
#define HIVER 3

#define ROCK 0
#define SAND 1
#define PIXEL 2
#define STAR 3
#define GROUND 4
#define JUPITER 5
#define JUPITER_NM 6


class TriangleWindow : public OpenGLWindow
{
public:
    TriangleWindow();

    void initialize() Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;

private:
    GLuint loadShader(GLenum type, const char *source);
    QImage loadTexture(char *filename, GLuint &textureID);
    void textureTriangle2D();
    void textureSol();
    void texturePyramide();
    void drawSphere();

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;
    GLuint tabTexture[7];

    QOpenGLShaderProgram *m_program;
    QOpenGLShaderProgram *m_diffuse;
    QOpenGLShaderProgram *m_SEM;
    QOpenGLShaderProgram *m_NM;
    QOpenGLShaderProgram *m_deform;

    int m_frame;
};

TriangleWindow::TriangleWindow()
    : m_program(0)
    , m_frame(0)
{
}

int main(int argc, char **argv)
{
    /*QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);

    TriangleWindow window;
    window.setFormat(format);
    window.resize(640, 480);
    window.show();

    window.setAnimating(true);

    return app.exec();*/

    srand(time(NULL));
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);


    Camera camera = Camera();

    /******************* PARTIE 2 CREATION DES SAISONS **********************/

    GameWindow printemps(60, &camera, PRINTEMPS);
    //printemps.setSeason(PRINTEMPS);
    printemps.setPosition(100, 50);
    printemps.setFormat(format);
    printemps.resize(640, 400);
    printemps.show();
    printemps.setTitle("PRINTEMPS");

    printemps.setAnimating(true);


    //    GameWindow ete(60, &camera, ETE);
    //    //ete.setSeason(ETE);
    //    ete.setPosition(700, 50);
    //    ete.setFormat(format);
    //    ete.resize(640, 400);
    //    ete.show();
    //    ete.setTitle("ETE");

    //    ete.setAnimating(true);

    //    GameWindow automne(60, &camera, AUTOMNE);
    //    //automne.setSeason(AUTOMNE);
    //    automne.setPosition(100, 350);
    //    automne.setFormat(format);
    //    automne.resize(640, 400);
    //    automne.show();
    //    automne.setTitle("AUTOMNE");

    //    automne.setAnimating(true);

        GameWindow hiver(60, &camera, HIVER);
        hiver.setSeason(HIVER);
        hiver.setPosition(700, 350);
        hiver.setFormat(format);
        hiver.resize(640, 400);
        hiver.show();
        hiver.setTitle("HIVER");

    //    hiver.setAnimating(true);

        // FIN SAISONS

        return app.exec();
}

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

GLuint TriangleWindow::loadShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    return shader;
}

void TriangleWindow::initialize()
{
    /*m_diffuse = new QOpenGLShaderProgram(this);
    m_diffuse->addShaderFromSourceFile(QOpenGLShader::Vertex, "C:\\Users\\Galaks\\Desktop\\tp5\\diffus_vertex_shader.vsh");
    m_diffuse->addShaderFromSourceFile(QOpenGLShader::Fragment, "C:\\Users\\Galaks\\Desktop\\tp5\\diffus_fragment_shader.fsh");

    if ( !m_diffuse->link() )
    {
        qWarning() << "Could not link";
        qDebug() << m_diffuse->log();
        return;
    }

    m_matrixUniform = m_diffuse->uniformLocation("matrix");*/

    /*m_SEM = new QOpenGLShaderProgram(this);
    m_SEM->addShaderFromSourceFile(QOpenGLShader::Vertex, "C:\\Users\\Galaks\\Desktop\\tp5\\SEM_vertex_shader.vsh");
    m_SEM->addShaderFromSourceFile(QOpenGLShader::Fragment, "C:\\Users\\Galaks\\Desktop\\tp5\\SEM_fragment_shader.fsh");

    if ( !m_SEM->link() )
    {
        qWarning() << "Could not link";
        qDebug() << m_SEM->log();
        return;
    }

    m_matrixUniform = m_SEM->uniformLocation("matrix");*/

    m_NM = new QOpenGLShaderProgram(this);
    m_NM->addShaderFromSourceFile(QOpenGLShader::Vertex, "C:\\Users\\Galaks\\Desktop\\tp5\\NM_vertex_shader.vsh");
    m_NM->addShaderFromSourceFile(QOpenGLShader::Fragment, "C:\\Users\\Galaks\\Desktop\\tp5\\NM_fragment_shader.fsh");

    if ( !m_NM->link() )
    {
        qWarning() << "Could not link";
        qDebug() << m_NM->log();
        return;
    }

    m_matrixUniform = m_NM->uniformLocation("matrix");

    /*m_deform = new QOpenGLShaderProgram(this);
    m_deform->addShaderFromSourceFile(QOpenGLShader::Vertex, "C:\\Users\\Galaks\\Desktop\\tp5\\Deform_vertex_shader.vsh");
    m_deform->addShaderFromSourceFile(QOpenGLShader::Fragment, "C:\\Users\\Galaks\\Desktop\\tp5\\Deform_fragment_shader.fsh");

    if ( !m_deform->link() )
    {
        qWarning() << "Could not link";
        qDebug() << m_deform->log();
        return;
    }*

    m_matrixUniform = m_deform->uniformLocation("matrix");*/
    //m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    //m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);



    //m_program->link();

    //m_posAttr = m_program->attributeLocation("posAttr");
    //m_colAttr = m_program->attributeLocation("colAttr");

    glEnable(GL_DEPTH_TEST);
    glScalef(0.3f, 0.3f, 0.3f);

    glEnable(GL_LIGHTING);	// Active l'éclairage
    glEnable(GL_LIGHT0);	// Allume la lumière n°1

    glEnable( GL_COLOR_MATERIAL );

    float ambient[] = {1 , 1 , 1 , 1.0};
    float diffuse[] = {1 , 1 , 1 , 1.0};
    float position[] = {3.0 , 14.0 , 20.0 , 1.0};

    glLightfv(GL_LIGHT0 , GL_AMBIENT , ambient);
    glLightfv(GL_LIGHT0 , GL_DIFFUSE , diffuse);
    glLightfv(GL_LIGHT0 , GL_POSITION , position);

    loadTexture("C:\\Users\\Galaks\\Desktop\\tp5\\rock.jpg", tabTexture[ROCK]);
    loadTexture("C:\\Users\\Galaks\\Desktop\\tp5\\sand.jpg", tabTexture[SAND]);
    loadTexture("C:\\Users\\Galaks\\Desktop\\tp5\\pixel.jpg", tabTexture[PIXEL]);
    loadTexture("C:\\Users\\Galaks\\Desktop\\tp5\\etoile.jpg", tabTexture[STAR]);
    loadTexture("C:\\Users\\Galaks\\Desktop\\tp5\\ground.jpg", tabTexture[GROUND]);
    loadTexture("C:\\Users\\Galaks\\Desktop\\tp5\\jupiter.jpg", tabTexture[JUPITER]);
    loadTexture("C:\\Users\\Galaks\\Desktop\\tp5\\jupiter_NM.png", tabTexture[JUPITER_NM]);

}

QImage TriangleWindow::loadTexture(char *filename, GLuint &textureID){
    glEnable(GL_TEXTURE_2D); // Enable texturing

    glGenTextures(1, &textureID); // Obtain an id for the texture
    glBindTexture(GL_TEXTURE_2D, textureID); // Set as the current texture

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    QImage im(filename);

    QImage tex = QGLWidget::convertToGLFormat(im);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    glDisable(GL_TEXTURE_2D);

    return tex;
}

void TriangleWindow::textureTriangle2D()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tabTexture[SAND]);

    glBegin(GL_TRIANGLES);
        glTexCoord2f(0,0); glVertex2f(-0.5f, 0.0f);
        glTexCoord2f(1,0); glVertex2f(0.0f, 1.0f);
        glTexCoord2f(1,1); glVertex2f(0.5f, 0.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void TriangleWindow::texturePyramide()
{
    glPushMatrix();
    glRotatef(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tabTexture[ROCK]);

    glBegin(GL_TRIANGLES);

    glTexCoord2f(0,0);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(1,0);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1,1);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, tabTexture[SAND]);

    glBegin(GL_TRIANGLES);


    glTexCoord2f(0,0);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(1,0);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1,1);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, tabTexture[PIXEL]);

    glBegin(GL_TRIANGLES);


    glTexCoord2f(0,0);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(1,0);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1,1);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, tabTexture[STAR]);

    glBegin(GL_TRIANGLES);


    glTexCoord2f(0,0);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(1,0);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1,1);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glEnd();

    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, tabTexture[GROUND]);

    glBegin(GL_QUADS);

    glTexCoord2f(1,1);
    glVertex3f(2.0f, -2.0f, 2.0f);
    glTexCoord2f(0,1);
    glVertex3f(-2.0f, -2.0f, 2.0f);
    glTexCoord2f(0,0);
    glVertex3f(-2.0f, -2.0f, -2.0f);
    glTexCoord2f(1,0);
    glVertex3f(2.0f, -2.0f, -2.0f);

    glEnd();


    glDisable(GL_TEXTURE_2D);
}

void TriangleWindow::textureSol()
{
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, tabTexture[GROUND]);

    glBegin(GL_QUADS);

    glTexCoord2f(1,1);
    glVertex3f(2.0f, -2.0f, 2.0f);
    glTexCoord2f(0,1);
    glVertex3f(-2.0f, -2.0f, 2.0f);
    glTexCoord2f(0,0);
    glVertex3f(-2.0f, -2.0f, -2.0f);
    glTexCoord2f(1,0);
    glVertex3f(2.0f, -2.0f, -2.0f);

    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void TriangleWindow::drawSphere()
{
    /*glUniform1i(m_SEM->uniformLocation("texture1"), 0);
    glUniform1i(m_SEM->uniformLocation("texture2"), 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tabTexture[JUPITER]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tabTexture[STAR]);*/

    glUniform1i(m_NM->uniformLocation("texture1"), 0);
    glUniform1i(m_NM->uniformLocation("texture2"), 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tabTexture[JUPITER]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tabTexture[JUPITER_NM]);

    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, tabTexture[JUPITER]);

//    glTexCoord2f(0,0);
    //glBindTexture(GL_TEXTURE_2D, tabTexture[JUPITER]);

    GLUquadricObj *quadric=gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricTexture(quadric, GL_TRUE);
    gluSphere(quadric, 1, 50,50);
    gluDeleteQuadric(quadric);

    //glDisable(GL_TEXTURE_2D);
}


void TriangleWindow::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //m_diffuse->bind();
    //m_SEM->bind();
    m_NM->bind();
    //m_deform->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -2);
    matrix.rotate(-50.0f * m_frame / screen()->refreshRate(), 0, 1, 0);
    matrix.rotate(-60.0f, 1, 0, 0);

    //m_diffuse->setUniformValue(m_matrixUniform, matrix);
    //m_SEM->setUniformValue(m_matrixUniform, matrix);
    m_NM->setUniformValue(m_matrixUniform, matrix);
    //m_deform->setUniformValue(m_matrixUniform, matrix);



    //glPushMatrix();
    //glRotatef(-45.0f, 1, 0, 0);

    //textureTriangle2D();
    //texturePyramide();
    //textureSol();

    drawSphere();


    //glPopMatrix();

    //m_diffuse->release();
    //m_SEM->release();
    m_NM->release();
    // m_deform->release();

    ++m_frame;
}
