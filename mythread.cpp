#include "mythread.h"

using namespace std;

MyThread::MyThread(Camera camera, char * name)
{
    this->camera = camera;
    this->name = name;
}

void MyThread::run()
{
    GameWindowClient client(60, &camera, name);
}

