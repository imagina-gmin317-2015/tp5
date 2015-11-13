#ifndef TRIANGLEWINDOW_H
#define TRIANGLEWINDOW_H

#include "openglwindow.h"

#include <gl/glu.h>
#include <QtGui/QGuiApplication>
#include <QtGui/QOpenGLShaderProgram>


#define PYRAMIDE 1
#define ECLAIRAGE_DIFFUS 2
#define SPHERICAL_ENVIRONMENT_MAPPING 3
#define NORMAL_MAPPING 4
#define GEOMETRY_DEFORMATION 5


class TriangleWindow : public OpenGLWindow
{
public:
    TriangleWindow(int type);

    void initialize() Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;


private:
    void loadTexture2(char *filename, GLuint &textureID);
    void textureTriangle2D();
    void texturePyramide();
    void sphereEclairageDiffus();
    void sphereSEM();
    void sphereNM();
    void sphereDEF();

    bool first;

    GLuint m_matrixUniform;
    GLuint m_cameraPosUniform;


    GLuint backgroundimage1;
    GLuint backgroundimage2;
    GLuint backgroundimage3;
    GLuint backgroundimage4;
    GLuint backgroundimage5;
    GLuint backgroundSphere;

    GLuint backgroundSEM;
    GLuint backgroundNM;

    QOpenGLShaderProgram *m_program;
    QOpenGLShaderProgram *m_programSEM;
    QOpenGLShaderProgram *m_programNM;
    QOpenGLShaderProgram *m_programDEF;

    int m_type;

    int m_frame;
};


#endif // TRIANGLEWINDOW_H


