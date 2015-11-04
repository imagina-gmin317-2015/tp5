#include "particule.h"

Particule::Particule(QObject *parent) : QObject(parent)
{

}

Particule::Particule(float x, float y, float ground, QTimer *timer)
{
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    this->x = x;
    this->y = y;
    this->z = 0.40;
    this->ground = ground;
    this->moveSpeed = (((float)(rand()) / (float)(RAND_MAX))/100) + 0.0001;
}

float Particule::getX() { return x; }
float Particule::getY() { return y; }
float Particule::getZ() { return z; }

void Particule::update()
{
    z -= moveSpeed;
    if(z < ground)
        z = 0.40;
}

Particule::~Particule()
{}
