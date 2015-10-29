#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>

#include <QtCore/qmath.h>

#include "trianglewindow.h"

int main(int argc, char **argv)
{
    QGuiApplication* app = new QGuiApplication(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);

    TriangleWindow window;
    window.setFormat(format);
    window.resize(640, 480);
    window.show();

    window.setAnimating(true);

    return app->exec();
}
