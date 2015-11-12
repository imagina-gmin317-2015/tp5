#include "triangletp5.h"


TriangleTP5::TriangleTP5(int choix)
    : m_program(0)
    , m_frame(0)
{
    choixShader = choix;
}

GLuint TriangleTP5::loadShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    return shader;
}

void TriangleTP5::initialize()
{

    m_program = new QOpenGLShaderProgram(this);
    switch(choixShader){
    case 1:
        m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSourceDiff);
        m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSourceDiff);
        break;
    case 2:
        m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSourceSEM);
        m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSourceSEM);
        break;
    case 3:
        m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSourceNorMap);
        m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSourceNorMap);
        break;
    case 4:
        m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSourceGeo);
        m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSourceGeo);
        break;
    }


    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");


    sphere = new Ply(":/sphere.ply");
    sphere->setPosX(0.5);
    sphere->setPosY(0.5);
    sphere->setPosZ(-1);

    sphere->setTaille(0.25);

    tex =(Texture**) malloc(sizeof(Texture*) * 6);
    initTexture2D(0,":/grass.png");
    //initTexture2D(0,":/normal.jpg");
    initTexture2D(1,":/box.png");
    initTexture2D(2,":/yellow.png");
    initTexture2D(3,":/red.png");
    initTexture2D(4,":/green.png");
    initTexture2D(5,":/blue.png");


    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -100.0, 100.0);


}

void TriangleTP5::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH);

    glLoadIdentity();
    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -5);
    matrix.rotate(90, 1, 0, 0);
    matrix.rotate(270, 0, 0, 1);
    //matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);
    //glRotatef(1,0,0,1);
    m_program->setUniformValue(m_matrixUniform, matrix);


    //displayTextureColor();
    displayTexture2D();
    displaySphere();

    m_program->release();

    ++m_frame;
}


void TriangleTP5::displayTextureColor(){


    GLfloat vertices[] = {
        0.0f, 0.707f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

void TriangleTP5::initTexture2D(int i, QString nomImage){
    tex[i] = new Texture(nomImage);
}

void TriangleTP5::displayTexture2D(){

    /*glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexture);
*/

    glClear(GL_COLOR_BUFFER_BIT);
    //Pyramide
    glPushMatrix();
    glScalef(0.5,0.5,0.5);
    glTranslatef(0,0.0,0.0);
    glRotatef(-75, 1, 0, 0);
    glRotatef(15, 0, 1, 0);
    glRotatef(m_frame%360, 0,0,1);
    tex[2]->initTexture();
    glBegin(GL_TRIANGLES);
            glTexCoord2f(0.5f, 1.0f); glVertex3f(0.5f, 0.0f,  1.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f,  0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f,  0.0f);
    glEnd();

    tex[3]->initTexture();
    glBegin(GL_TRIANGLES);
            glTexCoord2f(0.5f, 1.0f); glVertex3f(0.5f, 0.0f,  1.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f,  0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 0.0f,  0.0f);
    glEnd();

    tex[4]->initTexture();
    glBegin(GL_TRIANGLES);
            glTexCoord2f(0.5f, 1.0f); glVertex3f(0.5f, 0.0f,  1.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 0.0f,  0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f,  0.0f);

    glEnd();

    tex[5]->initTexture();
    glBegin(GL_TRIANGLES);
            glTexCoord2f(0.5f, 1.0f); glVertex3f(0.5f, 0.0f,  1.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, 0.5f,  0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f,  0.0f);

    glEnd();
     //La base
    tex[1]->initTexture();
    glBegin(GL_TRIANGLES);
            glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, 0.0f,  0.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f,  0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f,  0.0f);

            glTexCoord2f(0.5f, 1.0f); glVertex3f(0.5f, 0.5f,  0.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f,  0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 0.0f,  0.0f);


    glEnd();

    glPopMatrix();




    tex[0]->initTexture();
    //Sol
    glPushMatrix();
    glScalef(0.75,0.75,0.75);
    glTranslatef(-0.5,0.0,0.25);
    glRotatef(0, 0, 0, 1);
    glRotatef(0, 0, 1, 0);
    glRotatef(60, 1, 0, 0);

    glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f,  1.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 0.0f,  1.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f,  1.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 1.0f,  1.0f);
    glEnd();

    glPopMatrix();
}


void TriangleTP5::displaySphere(){
    sphere->drawFaces();
}

