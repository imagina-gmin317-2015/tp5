#include "openglwindow.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>

#include <QtCore/qmath.h>

#include <QOpenGLBuffer>
#include <QVector3D>
#include <QVector>
#include <QOpenGLTexture>

class TriangleWindow : public OpenGLWindow
{
public:
    TriangleWindow();

    static const int tailleX = 240;
    static const int tailleY = 240;

    QImage relief = QImage("/home/thibaut/Dropbox/M2-IMAGINA/MoteurJeux/tp5/heightmap-1.png");

    void initialize() Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;

private:
    GLuint loadShader(GLenum type, const char *source);

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;
    int m_frame;
    GLfloat getZ(GLfloat i, GLfloat j);
    GLfloat *getColors(GLint x, GLint y);
    GLfloat *getPoints(GLint x, GLint y);

    QVector<QVector3D> a_vertices;
    QOpenGLBuffer b_vertices;
    QOpenGLTexture *t_mosh;
};

TriangleWindow::TriangleWindow()
    : m_program(0)
    , m_frame(0)
{


//    GLfloat vertices[] = {
//        -0.2f, -0.0f, 0.0f,
//         0.2f, -0.0f, 0.0f,
//         0.0f,  0.4f, 0.0f
//    };

}
int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);

    TriangleWindow window;
    window.setFormat(format);
    window.resize(800, 600);
    window.show();

    window.setAnimating(true);

    return app.exec();
}

static const char *vertexShaderSource =
    //"attribute highp vec4 posAttr;\n"
    //"attribute lowp vec4 colAttr;\n"
    "varying vec4 pos;\n"
    "varying lowp vec4 col;\n"
    "uniform highp mat4 matrix;\n"
    "void main() {\n"
    "   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    //"   col = colAttr;\n"
    "   pos = gl_Vertex;\n"
    "}\n";

static const char *fragmentShaderSource =
    "uniform sampler2D tex;\n"
    "varying vec4 pos;\n"
    //"lowp vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = texture2D(tex,pos.xy);\n"
    "}\n";
//! [3]

//! [4]
GLuint TriangleWindow::loadShader(GLenum type, const char *source){
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    return shader;
}

void TriangleWindow::initialize(){
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");


    a_vertices.push_back(QVector3D{-0.4f, 0.0f, 0.0f});
    a_vertices.push_back(QVector3D{0.4f,  0.0f, 0.0f});
    a_vertices.push_back(QVector3D{0.0f,  0.8f, 0.0f});

    b_vertices.create();
    b_vertices.bind();
    b_vertices.allocate(a_vertices.constData(),a_vertices.size()*sizeof(QVector3D));
    b_vertices.release();

    QImage wall = QImage(":/wall3.jpg");//CHANGER IMAGE ICI
    t_mosh = new QOpenGLTexture(wall);
}

GLfloat TriangleWindow::getZ(GLfloat i, GLfloat j){
    return qGray(this->relief.pixel((this->tailleX * (i+0.5f)), (this->tailleY * (j+0.5f)))) * 0.0008f;
}

GLfloat *TriangleWindow::getColors(GLint x, GLint y){
    GLint nb = x * y * 3 * 2 + x * 3 + 3;
    GLfloat *couleurs= new GLfloat[nb];
    for(GLint i = 0;i<(nb);i++){
        couleurs[i] = 0.8f;
    }

    return couleurs;
}

GLfloat *TriangleWindow::getPoints(GLint x, GLint y){
    GLint nb = x * y * 3 * 2 + x * 3 + 3;
    GLfloat *points = new GLfloat[nb];
    GLfloat stepX = 1.0f / (x);
    GLfloat stepY = 1.0f / (y);
    GLint cpt = 0;
    GLint k = 1;
    GLfloat posX = -0.5f;
    GLfloat posY = -0.5f;

    for (GLint i = 0; i < x; ++i) {
        for (GLint j = 0; j < y; ++j) {
            points[cpt++] = posX;
            points[cpt++] = getZ(posX, posY);
            points[cpt++] = posY;
            points[cpt++] = posX + stepX;
            points[cpt++] = getZ(posX + stepX, posY);
            points[cpt++] = posY;
            posY += stepY * k;
        }
        points[cpt++] = posX;
        points[cpt++] = getZ(posX, posY);
        points[cpt++] = posY;
        k *= -1;
        posX += stepX;
    }
    points[cpt++] = posX;
    points[cpt++] = getZ(posX, posY);
    points[cpt++] = posY;

    return points;
}


void TriangleWindow::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

//    QMatrix4x4 matrix;
//    matrix.perspective(60.0f, 16.0f/9.0f, 0.1f, 100.0f);
//    matrix.translate(0, -0.13f, -0.6f);
//    matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glTranslatef(0,-0.13f,-0.6f);
    glRotatef(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

//    m_program->setUniformValue(m_matrixUniform, matrix);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


//    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, getPoints(tailleX, tailleY));
//    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, getColors(tailleX, tailleY));
//    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);
//    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

//    glEnableVertexAttribArray(0);
//    glEnableVertexAttribArray(1);

//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

//    glDisableVertexAttribArray(1);
//    glDisableVertexAttribArray(0);

    t_mosh->bind();

    glEnableClientState(GL_VERTEX_ARRAY);
    b_vertices.bind();
    glVertexPointer(3,GL_FLOAT,0,NULL);
    b_vertices.release();
    glDrawArrays(GL_TRIANGLES,0,a_vertices.size());
    glDisableClientState(GL_VERTEX_ARRAY);

    t_mosh->release();

    m_program->release();

    ++m_frame;
}
//! [5]
