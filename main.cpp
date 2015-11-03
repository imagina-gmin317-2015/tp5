#include "gamewindow.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QMainWindow>

#include <QtCore/qmath.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <time.h>
#include <sys/time.h>
#include <iostream>

#include <QtCore>
#include <QtGui>
#include <QPushButton>
#include <QApplication>
#include <QWidget>
#include <QObject>
#include <QThread>

#include "camera.h"
#include "serverthread.h"
#include "controllerwindow.h"
#include "resourcemanager.h"
using namespace std;

#define SERVER 1
#define CLIENT 0

#define SUMMER "summer"
#define AUTUMN "autumn"
#define WINTER "winter"
#define SPRING "spring"



GameWindow *createWindow(Camera* camera, float framerate) {
    QSurfaceFormat format;
    format.setSamples(16);

    GameWindow *w = new GameWindow(camera, framerate);
    w->setFormat(format);
    w->resize(1366, 1024);
    w->show();
    w->setAnimating(true);
    return w;
}


int main(int argc, char **argv)
{
    srand(time(NULL));
    QApplication app(argc, argv);

    ResourceManager::init();

    ControllerWindow window;
    window.show();
    window.resize(200, 120);
    window.move(1100, 400);

    Camera *c = new Camera();
    GameWindow *g = createWindow(c, 1.0f / 60.0f);
    g->setPosition(120, 10);

    QObject::connect(g, SIGNAL(requestLoad()), &window, SLOT(onLoadRequest()));
    QObject::connect(g, SIGNAL(requestSave()), &window, SLOT(onSaveRequest()));
    QObject::connect(&window, SIGNAL(requestLoad()), g, SLOT(onLoadRequest()));
    QObject::connect(&window, SIGNAL(requestSave()), g, SLOT(onSaveRequest()));

//    g = createWindow(c, 1.0f / 60.0f);
//    g->setPosition(640, 10);

//    QObject::connect(g, SIGNAL(requestLoad()), &window, SLOT(onLoadRequest()));
//    QObject::connect(g, SIGNAL(requestSave()), &window, SLOT(onSaveRequest()));
//    QObject::connect(&window, SIGNAL(requestLoad()), g, SLOT(onLoadRequest()));
//    QObject::connect(&window, SIGNAL(requestSave()), g, SLOT(onSaveRequest()));

//    g = createWindow(c, 1.0f / 60.0f);
//    g->setPosition(640, 400);

//    QObject::connect(g, SIGNAL(requestLoad()), &window, SLOT(onLoadRequest()));
//    QObject::connect(g, SIGNAL(requestSave()), &window, SLOT(onSaveRequest()));
//    QObject::connect(&window, SIGNAL(requestLoad()), g, SLOT(onLoadRequest()));
//    QObject::connect(&window, SIGNAL(requestSave()), g, SLOT(onSaveRequest()));

//    g = createWindow(c, 1.0f / 60.0f);
//    g->setPosition(120, 400);

//    QObject::connect(g, SIGNAL(requestLoad()), &window, SLOT(onLoadRequest()));
//    QObject::connect(g, SIGNAL(requestSave()), &window, SLOT(onSaveRequest()));
//    QObject::connect(&window, SIGNAL(requestLoad()), g, SLOT(onLoadRequest()));
//    QObject::connect(&window, SIGNAL(requestSave()), g, SLOT(onSaveRequest()));

    return app.exec();
}



