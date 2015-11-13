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

#include "tcpserver.h"
//#include "gamewindow.h"
#include "trianglewindow.h"
#include "camera.h"
#include "point.h"

using namespace std;


int main(int argc, char **argv)
{
    srand(time(NULL));
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);
    format.setDepthBufferSize(1);


    // TP 5
    TriangleWindow window(640, 480);
    window.setFormat(format);
    window.resize(640, 480);
    window.show();

    window.setAnimating(true);

//    TcpServer server;

//    Camera* c=new Camera();

//    GameWindow window(1000, 9999);
//    window.c = c;
//    window.setFormat(format);
//    window.resize(500,375);
//    window.setPosition(0,0);
//    window.show();

//    GameWindow window2(1000, 9999);
//    window2.c = c;
//    window2.setFormat(format);
//    window2.resize(500,375);
//    window2.setPosition(500, 0);
//    window2.show();

//    GameWindow window3(1000, 9999);
//    window3.c = c;
//    window3.setFormat(format);
//    window3.resize(500,375);
//    window3.setPosition(0, 450);
//    window3.show();

//    GameWindow window4(1000, 9999);
//    window4.c = c;
//    window4.setFormat(format);
//    window4.resize(500,375);
//    window4.setPosition(500,450);
//    window4.show();

    return app.exec();
}
