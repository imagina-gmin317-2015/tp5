#include "fall.h"

Fall::Fall(QObject *parent) : QObject(parent)
{

}

Fall::Fall(QVector<QVector3D> particules, QVector<float> ground, QVector<float> fallSpeed, QTimer *timer)
{
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    _particules = particules;
    _grounds = ground;
    _fallSpeed = fallSpeed;
    _pointColor = QVector4D(1.0, 1.0, 1.0, 0.5);
    _pointSize = 3.0;
    _faster = 0.0;
}

QVector<QVector3D> Fall::getParticules()
{
    return _particules;
}

void Fall::update()
{
    for(int i = 0; i < MAX_PARTICULES; i++)
    {
        float y = _particules[i].y();
        float fallSpeed = _fallSpeed[i];
        float ground = _grounds[i];
        _particules[i].setY(y-(fallSpeed + _faster));
        if(_particules[i].y() <= ground)
        {
            _particules[i].setY(PARTICULE_MAX_Y);
        }
    }
}

QVector4D Fall::getPointColor()
{
    return _pointColor;
}

float Fall::getPointSize()
{
    return _pointSize;
}

void Fall::setPointColor(QVector4D color)
{
    _pointColor = color;
}

void Fall::setPointSize(float size)
{
    _pointSize = size;
}

void Fall::setFaster(float faster)
{
    _faster = faster;
}

Fall::~Fall()
{}
