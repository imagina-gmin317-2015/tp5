#include "camera.h"
#include <QtGui/QOpenGLShaderProgram>

Camera::Camera():
    m_etat(0)
    ,m_rotX(-135.0)
    ,m_rotY(-45.0)
    ,m_ss(1.0f)
{}

float Camera::getSS()
{
    return this->m_ss;
}

float Camera::getRotX()
{
    return this->m_rotX;
}

float Camera::getRotY()
{
    return this->m_rotY;
}

int Camera::getEtat()
{
    return this->m_etat;
}


void Camera::setSS(float ss)
{
    this->m_ss = ss;
}

void Camera::setRotX(float rotX)
{
    this->m_rotX = rotX;
}

void Camera::setRotY(float rotY)
{
    this->m_rotY = rotY;
}

void Camera::setEtat(int etat)
{
    this->m_etat = etat;
}

void Camera::scale()
{
    glScalef(m_ss, m_ss, m_ss);
}
