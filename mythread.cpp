#include "mythread.h"
#include "gamewindowserveur.h"
#include "gamewindowclient.h"

#include <QSurfaceFormat>

MyThread::MyThread(Camera camera, char * name):
    m_camera(camera),
    m_name(name)
{

}

void MyThread::run()
{
    QSurfaceFormat format;
    format.setSamples(16);

    GameWindowClient clientWindow (50,&m_camera,m_name);
    clientWindow.setFormat(format);
    clientWindow.resize(640, 480);
    clientWindow.show();


}

