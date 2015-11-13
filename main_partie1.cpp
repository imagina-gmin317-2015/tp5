
#include "glwindow.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>

#include <QtCore/qmath.h>

#include <QPainter>
#include <QOpenGLTexture>

class TriangleWindow : public GlWindow
{
public:
    TriangleWindow();

    void initialize() Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;

private:
    GLuint loadShader(GLenum type, const char *source);

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;
    int m_frame;
};

TriangleWindow::TriangleWindow()
    : m_program(0)
    , m_frame(0)
{
}

/****************************************** /
int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);

    TriangleWindow window;
    window.setFormat(format);
    window.resize(640, 480);
    window.show();

    window.setAnimating(true);

    return app.exec();
}
/****************************************/

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
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
}

void TriangleWindow::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -4);
    matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    m_program->setUniformValue(m_matrixUniform, matrix);

    GLfloat PyramideVertices[] = {
        0.0f, 0.5f, 0,//DEVANT
        -0.5f, -0.5f, 0.5,
        0.5f, -0.5f, 0.5,
        0.0f, 0.5f, 0,//Derriere
        -0.5, -0.5, -0.5,
        0.5f, -0.5f, -0.5,
        0.0f, 0.5f, 0,//Gauche
        -0.5f, -0.5f, 0.5,
        -0.5f, -0.5f, -0.5,
        0.0f, 0.5f, 0,//Droite
        0.5f, -0.5f, 0.5,
        0.5f, -0.5f, -0.5
    };

    GLfloat soilVertices[]= {
        1,-1,1,
        1,-1,-1,
        -1,-1,1,
        -1,-1,-1
    };

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0,0,1,
        0,0,1,
        0,0,1,
        1,1,1,
        1,1,1,
        1,1,1
    };

    //glBindTexture(GL_TEXTURE_2D, texture.bind);
    //LoadTexture(QString(":/pyramide.jpg"));
    /************************************************** /

    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, PyramideVertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 12);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    matrix = QMatrix4x4();
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -4);
    m_program->setUniformValue(m_matrixUniform, matrix);

    /************************************************* /
    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, soilVertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_QUAD_STRIP, 0, 4);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    /*************************************************/


    /*QImage imageTexture (QString(":/pyramide.jpg"));
    QOpenGLTexture* texture = new QOpenGLTexture(imageTexture.mirrored());
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->bind();*/

    glPointSize(1);
    glBegin(GL_POINTS);
    glColor3f(0,1,0);
        glVertex3f(1,-1,1);
        glVertex3f(1,-1,-1);
        glVertex3f(-1,-1,1);
        glVertex3f(-1,-1,-1);
    glEnd();

    m_program->release();

    ++m_frame;
}
