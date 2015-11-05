#ifndef TRIANGLEWINDOW_H
#define TRIANGLEWINDOW_H

#include "openglwindow.h"
#include "point.h"
#include "particule.h"

#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QVector>
#include <iostream>
#include <QFile>
#include <QKeyEvent>
#include <math.h>

#define MAX_PARTICULES 700

class TriangleWindow : public OpenGLWindow
{
public:
    TriangleWindow();
    TriangleWindow(int, int);

    void initialize() Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *keyEvent);
    void generateTerrain();
    QVector3D displayColor(float alt);

private:
    GLuint loadShader(GLenum type, const char *source);

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_texAttr;
    GLuint m_texCoord;
    GLuint m_normal;
    GLuint m_matrixUniform;

    int _width;
    int _height;
    GLfloat _camX;
    GLfloat _camY;
    GLfloat _camZ;
    GLfloat _angle;

    bool _wireFrame;

    QVector<QVector3D> _map;
    QVector<QVector3D> _normal;
    QVector<GLfloat> _texture;
    QVector<QVector3D> _color;

    QOpenGLShaderProgram *m_program;
    int m_frame;

    QOpenGLTexture *texture;

    // vao
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    QOpenGLBuffer matVbo;

    //seasons
    QTimer *timerParticule;
    QString *allSeasons;

    int currentSeason;

    Particule **particules;
};

#endif // TRIANGLEWINDOW_H
