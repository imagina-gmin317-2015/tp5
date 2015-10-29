#ifndef TRIANGLEWINDOW_H
#define TRIANGLEWINDOW_H

#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtCore/qmath.h>
#include <QtGui/QScreen>

#include <QKeyEvent>

#include "openglwindow.h"
#include "texture.h"
#include "point.h"

typedef unsigned int uint;

class TriangleWindow : public OpenGLWindow
{
public:
    TriangleWindow();

    void initialize() Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *event);

private:
    GLuint loadShader(GLenum type, const char *source);
    void DrawCurve(Point tabPointsOfCurve[], long nbPoints);
    void DrawSphere(float rayon, Point position, unsigned int m, unsigned int p);

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    GLuint m_light_position;
    GLuint m_light_diffuse;

    GLuint m_intensity;

    QOpenGLShaderProgram *m_program;
    int m_frame;

    Texture* texture;
    float intensity;
};

#endif // TRIANGLEWINDOW_H
