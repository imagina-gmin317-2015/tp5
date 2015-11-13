#ifndef TRIANGLEWINDOW_H
#define TRIANGLEWINDOW_H
#include "openglwindow.h"
#include "camera.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>

#include <QtCore/qmath.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <time.h>
#include <sys/time.h>
#include <iostream>

#include <QtCore>
#include <QtGui>

#include <QtOpenGL/QGLFormat>
#include "arraytools.h"

struct point3D
{
    float x, y ,z;
};
struct point2D
{
    float x, y;
};

class TriangleWindow : public OpenGLWindow
{
    Q_OBJECT
public:
    TriangleWindow();
    TriangleWindow(Camera *cam);
    ~TriangleWindow();

    void initialize() Q_DECL_OVERRIDE;
//    void render() Q_DECL_OVERRIDE;
    void initPyramid();
    void initMap(QString localPath);
    void initPart(int nPart);
    const float* buildArrayFromIndex(QVector<point3D> obj, QVector<GLint> index);
    const float* buildArrayFromIndex(QVector<point2D> obj, QVector<GLint> index);
    bool event(QEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    GLuint loadShader(GLenum type, const char *source);
// La pyramide
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;
    GLuint m_texAttr;
    GLuint buf;         /* identifiant de notre objet tampon */
    QOpenGLShaderProgram *m_program;
    int m_frame;


    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo[3];

// La map

    GLuint map_posAttr;
    GLuint map_normals;
    GLuint map_matrixUniform;
    GLuint map_light;
    GLuint map_ambient;
    GLuint map_buf;         /* identifiant de notre objet tampon */
    GLint mapLength;
    QOpenGLShaderProgram *map_program;

    QOpenGLVertexArrayObject map_vao;
    QOpenGLBuffer map_vbo[2];
    QImage m_image;
    QVector<QVector3D> position;

// Les particules
    GLuint part_posAttr;
    GLuint part_matrixUniform;
//    GLuint map_buf;         /* identifiant de notre objet tampon */
    QOpenGLShaderProgram *part_program;

    QOpenGLVertexArrayObject part_vao;
    QOpenGLBuffer part_vbo[2];
    QVector<QVector3D> particles;
    QVector<float> floor;
    QVector<float> speed;
    QTimer *part_timer;




    QOpenGLShaderProgram *program;
    GLuint pos,col,t1, t2, tex, map_pos;
    QVector<QOpenGLTexture*> texture;

    Camera *cam;

public slots:
    void updatePart();
    void render() Q_DECL_OVERRIDE;

};




#endif // TRIANGLEWINDOW_H
