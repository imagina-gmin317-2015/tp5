#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "gamewindow.h"
#include "Camera.h"

#include <QThread>

class MyThread : public QThread
{

private:
    Camera m_camera;
    char * m_name;

public:
    MyThread(Camera camera,char * name);
    virtual void run();

};

#endif // MYTHREAD_H
