#include "Camera.h"

#include <QtGui/QOpenGLShaderProgram>

Camera::Camera():
    m_etat(0),
    m_rotX(-135.0),
    m_rotY(-135.0),
    m_ss(1.0f)
{
    
}

void Camera::scale(){
    glScalef(m_ss,m_ss,m_ss);
}

//Getteurs
int Camera::get_etat(){
    return this->m_etat;
}

float Camera::get_rotX(){
    return this->m_rotX;
}

float Camera::get_rotY(){
    return this->m_rotY;
}

float Camera::get_ss(){
    return this->m_ss;
}

//Setteurs

void Camera::set_etat(int etat){
    this->m_etat = etat;
}

void Camera::set_rotX(float rotX){
    this->m_rotX = rotX;
}
void Camera::set_rotY(float rotY){
    this->m_rotY = rotY;
}

void Camera::set_ss(float ss){
    this->m_ss = ss;
}
