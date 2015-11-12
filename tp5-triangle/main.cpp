#include "trianglewindow.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QtGui/QOpenGLFunctions>

#include <QtCore/qmath.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <time.h>
#include <sys/time.h>
#include <iostream>

#include <QtCore>
#include <QtGui>
#include <QGLFormat>

#include <omp.h>

using namespace std;

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);
    format.setMajorVersion(2);
    format.setMinorVersion(0);
    format.setDepthBufferSize(24);
    format.setProfile(QSurfaceFormat::CoreProfile);


    TriangleWindow window;
    window.setSurfaceType(QWindow::SurfaceType::OpenGLSurface);
    window.setFormat(format);

    window.resize(640, 480);
    window.show();

    window.setAnimating(true);

    return app.exec();
}
