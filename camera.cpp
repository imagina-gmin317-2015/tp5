#include "camera.h"

Camera::Camera()
{

}

void Camera::initialize(qreal ratio, qreal width, qreal height, qreal near, qreal far)
{
    glViewport(0, 0, width * ratio, height * ratio);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1, 100);
    this->rotX = 300;
    this->rotY = 0;
    this->rotZ = 40;
    this->ss = 2;
}

void Camera::rotate(float x, float y, float z)
{
    this->rotX += x;
    this->rotY += y;
    this->rotZ += z;
}

void Camera::scale(float scaleX, float scaleY, float scaleZ)
{
    this->ss += scaleX;
}

void Camera::translate(float x, float y, float z)
{
    this->x += x;
    this->y += y;
    this->z = z;
}

void Camera::update(float delta)
{
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -10, 100);

    glScalef(ss,ss,ss);
    //    glTranslatef(y, x, z);
    glRotatef(rotX,1.0f,0.0f,0.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotZ, 0, 0, 1);

    if(this->animated) {
        this->rotate(0, 0, 0.3);
    }
}

void Camera::setAnimated(bool b)
{
    this->animated = b;
}

float Camera::getRotationX() const
{
    return this->rotX;
}

float Camera::getRotationY() const
{
    return this->rotY;
}

float Camera::getRotationZ() const
{
    return 0;
}

float Camera::getScaleX() const
{
    return this->ss;
}

float Camera::getScaleY() const
{
    return this->ss;
}

bool Camera::isAnimated() const
{
    return this->animated;
}

float Camera::getX() const
{
    return x;
}

float Camera::getY() const
{
    return y;
}

void Camera::setCursorCaptured(bool b)
{
    this->cursorCaptured = b;
}

bool Camera::isCursorCaptured() const
{
    return this->cursorCaptured;
}

QVector<data *> Camera::attributes()
{
    QVector<data *> att;
    att.push_back(ResourceManager::INT(&etat));
    att.push_back(ResourceManager::FLOAT(&rotX));
    att.push_back(ResourceManager::FLOAT(&rotY));
    att.push_back(ResourceManager::FLOAT(&rotZ));
    att.push_back(ResourceManager::FLOAT(&ss));
    att.push_back(ResourceManager::FLOAT(&x));
    att.push_back(ResourceManager::FLOAT(&y));
    att.push_back(ResourceManager::FLOAT(&z));
    att.push_back(ResourceManager::BOOL(&animated));
    att.push_back(ResourceManager::BOOL(&cursorCaptured));
    return att;
}
