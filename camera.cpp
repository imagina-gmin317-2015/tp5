#include "camera.h"

#include <QtGui/QGuiApplication>



#include <sys/time.h>
#include <iostream>

#include <QtCore>
#include <QtGui>

Camera::Camera(QObject *parent )
    : QObject(parent)
{

}

Camera::~Camera()
{}



void Camera::rotateY()
{
    //Camera::update();
    if (toRot) rotY+=1.0f;
}


void Camera::update()
{
    rotY+=1.0f;
}
