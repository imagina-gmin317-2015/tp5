#include <QCoreApplication>
#include "openglwindow.h"
#include "trianglewindow.h"
#include "camera.h"

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);
    
    Camera *cam = new Camera();
    
    QSurfaceFormat format;
    format.setSamples(16);
    format.setDepthBufferSize(1);
   format.setVersion(3,3);

    TriangleWindow window(cam);
    window.setFormat(format);

    window.resize(640, 480);
    window.show();

    window.setAnimating(false);
    return a.exec();
}

