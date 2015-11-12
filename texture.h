#ifndef TEXTURE
#define TEXTURE

#include "openglwindow.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QtGui/QOpenGLTexture>

#include <QtCore/qmath.h>
#include <QGLWidget>
#include <QFile>

class Texture{
public:
    Texture();
    Texture(QString);

    void initTexture();
    void delTexture();

    QString getNom();
    void setNom(QString);
    GLuint getId();
    void setId(GLuint);

private:
    QString nom;
    GLuint id;    
    QImage texture;
};

#endif // TEXTURE

