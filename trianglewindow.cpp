#include "trianglewindow.h"
#include "openglwindow.h"

#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QtOpenGL/QGLWidget>
#include <QtCore/qmath.h>

TriangleWindow::TriangleWindow(int type)
    : m_program(0)
    , m_frame(0)
    , m_type(type)
{
}

void TriangleWindow::initialize()
{

    first = true;

    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "..\\tp5\\Shaders\\vertexShader.vsh");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "..\\tp5\\Shaders\\fragmentShader.fsh");

    m_programSEM = new QOpenGLShaderProgram(this);
    m_programSEM->addShaderFromSourceFile(QOpenGLShader::Vertex, "..\\tp5\\Shaders\\SEM_vertexShader.vsh");
    m_programSEM->addShaderFromSourceFile(QOpenGLShader::Fragment, "..\\tp5\\Shaders\\SEM_fragmentShader.fsh");

    m_programNM = new QOpenGLShaderProgram(this);
    m_programNM->addShaderFromSourceFile(QOpenGLShader::Vertex, "..\\tp5\\Shaders\\NM_vertexShader.vsh");
    m_programNM->addShaderFromSourceFile(QOpenGLShader::Fragment, "..\\tp5\\Shaders\\NM_fragmentShader.fsh");

    m_programDEF = new QOpenGLShaderProgram(this);
    m_programDEF->addShaderFromSourceFile(QOpenGLShader::Vertex, "..\\tp5\\Shaders\\DEF_vertexShader.vsh");
    m_programDEF->addShaderFromSourceFile(QOpenGLShader::Fragment, "..\\tp5\\Shaders\\DEF_fragmentShader.fsh");

    glEnable(GL_DEPTH_TEST);

    glScalef(0.3f,0.3f,0.3f);

    glEnable(GL_LIGHTING);	// Active l'éclairage
    glEnable(GL_LIGHT0);	// Allume la lumière n°1

    glEnable( GL_COLOR_MATERIAL );

    float ambient[] = {1 , 1 , 1 , 1.0};
    float diffuse[] = {1 , 1 , 1 , 1.0};
    float position[] = {3.0 , 14.0 , 20.0 , 1.0};

    glLightfv(GL_LIGHT0 , GL_AMBIENT , ambient);
    glLightfv(GL_LIGHT0 , GL_DIFFUSE , diffuse);
    glLightfv(GL_LIGHT0 , GL_POSITION , position);

}

void TriangleWindow::loadTexture2(char *filename, GLuint &textureID){
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

}

void TriangleWindow::textureTriangle2D()
{
    if(first){
        if(!m_program->link()){
            qDebug() << "Erreur de link !" << endl;
            qDebug() << m_program->log() << endl;
            exit(0);
        }

        m_matrixUniform = m_program->uniformLocation("matrix");

        loadTexture2("..\\tp5\\Texture\\textureSandstone.png", backgroundimage5);
        first = false;
    }

    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -3);
    //matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    m_program->setUniformValue(m_matrixUniform, matrix);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, backgroundimage5);

    glBegin(GL_TRIANGLES);
        glTexCoord2f(0,0); glVertex2f(-0.5f, 0.0f);
        glTexCoord2f(1,0); glVertex2f(0.0f, 1.0f);
        glTexCoord2f(1,1); glVertex2f(0.5f, 0.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    m_program->release();

    ++m_frame;
}

void TriangleWindow::texturePyramide(){

    if(first){

        if(!m_program->link()){
            qDebug() << "Erreur de link !" << endl;
            qDebug() << m_program->log() << endl;
            exit(0);
        }

        m_matrixUniform = m_program->uniformLocation("matrix");

        loadTexture2("..\\tp5\\Texture\\texture7.png", backgroundimage1);
        loadTexture2("..\\tp5\\Texture\\texture.png", backgroundimage2);
        loadTexture2("..\\tp5\\Texture\\texture5.jpg", backgroundimage3);
        loadTexture2("..\\tp5\\Texture\\texture6.jpg", backgroundimage4);
        loadTexture2("..\\tp5\\Texture\\textureSandstone.png", backgroundimage5);

        first = false;
    }

    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -7);
    matrix.rotate(-100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    m_program->setUniformValue(m_matrixUniform, matrix);

    //pyramide

    glBindTexture(GL_TEXTURE_2D, backgroundimage1);

    glBegin(GL_TRIANGLES);
        glTexCoord2f(0,0);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(1,0);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.5f,1);
        glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();


    glBindTexture(GL_TEXTURE_2D, backgroundimage2);

    glBegin(GL_TRIANGLES);
        glTexCoord2f(0,0);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(1,0);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1,1);
        glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();


    glBindTexture(GL_TEXTURE_2D, backgroundimage3);

    glBegin(GL_TRIANGLES);
        glTexCoord2f(0,0);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(1,0);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glTexCoord2f(1,1);
        glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, backgroundimage4);

    glBegin(GL_TRIANGLES);
        glTexCoord2f(0,0);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(1,0);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glTexCoord2f(1,1);
        glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();


    matrix = QMatrix4x4();
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -7);

    m_program->setUniformValue(m_matrixUniform, matrix);

    glBindTexture(GL_TEXTURE_2D, backgroundimage5);

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

    m_program->release();
    ++m_frame;
}

void TriangleWindow::sphereEclairageDiffus(){

    if(first){
        if(!m_program->link()){
            qDebug() << "Erreur de link !" << endl;
            qDebug() << m_program->log() << endl;
            exit(0);
        }

        m_matrixUniform = m_program->uniformLocation("matrix");

        loadTexture2("..\\tp5\\Texture\\terre.jpg", backgroundSphere);
        first = false;
    }

    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -3);
    matrix.rotate(-100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    matrix.rotate(-180.0f, 0, 0, 1);
    matrix.rotate(90.0f, 1, 0, 0);

    m_program->setUniformValue(m_matrixUniform, matrix);

    //glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, backgroundSphere);

    GLUquadric* params = gluNewQuadric();

    gluQuadricDrawStyle(params,GLU_FILL);
    gluQuadricTexture(params,GL_TRUE);
    gluSphere(params,1,20,20);

    gluDeleteQuadric(params);

    //glDisable(GL_TEXTURE_2D);

    m_program->release();
    ++m_frame;
}

void TriangleWindow::sphereSEM(){

    if(first){
        if(!m_programNM->link()){
            qDebug() << "Erreur de link !" << endl;
            qDebug() << m_programNM->log() << endl;
            exit(0);
        }

        m_matrixUniform = m_programNM->uniformLocation("matrix");

        loadTexture2("..\\tp5\\Texture\\terre.jpg", backgroundSphere);
        loadTexture2("..\\tp5\\Texture\\SEM.jpg", backgroundSEM);

        first = false;
    }

    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    m_programSEM->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -3);
    matrix.rotate(-50.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    matrix.rotate(-180.0f, 0, 0, 1);
    matrix.rotate(90.0f, 1, 0, 0);

    m_programSEM->setUniformValue(m_matrixUniform, matrix);

    glUniform1i(m_programSEM->uniformLocation("texture1"), 0);
    glUniform1i(m_programSEM->uniformLocation("texture2"), 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, backgroundSphere);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, backgroundSEM);

    GLUquadric* params = gluNewQuadric();

    gluQuadricDrawStyle(params,GLU_FILL);
    gluQuadricTexture(params,GL_TRUE);
    gluSphere(params,1,20,20);

    gluDeleteQuadric(params);

    m_programSEM->release();
    ++m_frame;
}


void TriangleWindow::sphereNM(){

    if(first){
        if(!m_programNM->link()){
            qDebug() << "Erreur de link !" << endl;
            qDebug() << m_programNM->log() << endl;
            exit(0);
        }

        m_matrixUniform = m_programNM->uniformLocation("matrix");

        loadTexture2("..\\tp5\\Texture\\terre.jpg", backgroundSphere);
        loadTexture2("..\\tp5\\Texture\\NormalMap.png", backgroundNM);

        first = false;
    }

    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    m_programNM->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -3);
    matrix.rotate(-50.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    matrix.rotate(-180.0f, 0, 0, 1);
    matrix.rotate(90.0f, 1, 0, 0);

    m_programNM->setUniformValue(m_matrixUniform, matrix);

    glUniform1i(m_programNM->uniformLocation("texture1"), 0);
    glUniform1i(m_programNM->uniformLocation("texture2"), 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, backgroundSphere);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, backgroundNM);

    GLUquadric* params = gluNewQuadric();

    gluQuadricDrawStyle(params,GLU_FILL);
    gluQuadricTexture(params,GL_TRUE);
    gluSphere(params,1,20,20);

    gluDeleteQuadric(params);

    m_programNM->release();
    ++m_frame;
}

void TriangleWindow::sphereDEF(){

    if(first){
        if(!m_programDEF->link()){
            qDebug() << "Erreur de link !" << endl;
            qDebug() << m_programDEF->log() << endl;
            exit(0);
        }

        m_matrixUniform = m_programDEF->uniformLocation("matrix");

        loadTexture2("..\\tp5\\Texture\\terre.jpg", backgroundSphere);
        first = false;
    }

    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_programDEF->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -3);
    matrix.rotate(-100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    matrix.rotate(-180.0f, 0, 0, 1);
    matrix.rotate(90.0f, 1, 0, 0);

    m_programDEF->setUniformValue(m_matrixUniform, matrix);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, backgroundSphere);

    GLUquadric* params = gluNewQuadric();

    gluQuadricDrawStyle(params,GLU_FILL);
    gluQuadricTexture(params,GL_TRUE);
    gluSphere(params,1,20,20);

    gluDeleteQuadric(params);

    glDisable(GL_TEXTURE_2D);

    m_programDEF->release();
    ++m_frame;
}

/*
void TriangleWindow::sphereDEF(){

    if(first){
        if(!m_programDEF->link()){
            qDebug() << "Erreur de link !" << endl;
            qDebug() << m_programDEF->log() << endl;
            exit(0);
        }

        m_matrixUniform = m_programDEF->uniformLocation("matrix");
        m_cameraPosUniform = m_programDEF->uniformLocation("CAMERA_POSITION");

        loadTexture2("..\\tp5\\Texture\\terre.jpg", backgroundSphere);
        loadTexture2("..\\tp5\\Texture\\NormalMap.png", backgroundNM);

        first = false;
    }

    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    m_programDEF->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -3);
    matrix.rotate(-50.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    //matrix.rotate(-180.0f, 0, 0, 1);
    //matrix.rotate(90.0f, 1, 0, 0);

    QVector3D vect (0.0f,0.0f,-3.0f);

    m_programDEF->setUniformValue(m_matrixUniform, matrix);
    m_programDEF->setUniformValue(m_cameraPosUniform, vect);

    glUniform1i(m_programDEF->uniformLocation("basetex"), 0);
    glUniform1i(m_programDEF->uniformLocation("heightMap"), 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, backgroundSphere);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, backgroundNM);

    GLUquadric* params = gluNewQuadric();

    gluQuadricDrawStyle(params,GLU_FILL);
    gluQuadricTexture(params,GL_TRUE);
    gluSphere(params,1,20,20);

    gluDeleteQuadric(params);

    m_programNM->release();
    ++m_frame;
}
*/
void TriangleWindow::render()
{

    //textureTriangle2D();
    if(m_type == PYRAMIDE)
        texturePyramide();
    else if(m_type == ECLAIRAGE_DIFFUS)
        sphereEclairageDiffus();
    else if(m_type == SPHERICAL_ENVIRONMENT_MAPPING)
        sphereSEM();
    else if(m_type == NORMAL_MAPPING)
        sphereNM();
    else if(m_type == GEOMETRY_DEFORMATION)
        sphereDEF();

}
