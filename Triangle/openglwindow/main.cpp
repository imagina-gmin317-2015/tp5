#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>

#include <QtCore/qmath.h>

#include "trianglewindow.h"
#include "terrain.h"

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

    /*Terrain terrain("./heightmap-1.png", app);

    terrain.setFormat(format);
    terrain.resize(640, 480);
    terrain.show();

    terrain.setAnimating(true);*/

    return app->exec();
}
