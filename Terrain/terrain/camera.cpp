#include "camera.h"

Camera::Camera()
{
    if(FileManager::Instance().camera != nullptr){
        Camera* cam = FileManager::Instance().camera;
        etat = cam->etat;
        rotX = cam->rotX;
        rotY = cam->rotY;
        ss = cam->ss;
    }else{
        etat = 2;
        rotX = -45.f;
        rotY = -45.f;
        ss = 1.f;
    }

}

