#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include "gamewindowclient.h"
#include "gamewindowserver.h"
#include "camera.h"

class MyThread : public QThread
{
public:
    MyThread(Camera camera, char * name);
    void run();

private:
    Camera camera;
    char * name;

signals:

public slots:
};

#endif // MYTHREAD_H
