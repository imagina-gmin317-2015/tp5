#ifndef DROUGHT_H
#define DROUGHT_H

#include <QtGui/QOpenGLFunctions>
#include <QDebug>

#include "resourcemanager.h"
#include "birds.h"

class Drought
{
public:
    Drought();
    void update(float delta);
    void draw();
    float getYellow() const;
    float getSnowHeightModifier() const;
    void setActive(bool active);
    bool active() const;

    QVector<data *> attributes();

private:
    float yellow;
    float snowHeight;
    bool isActive;
    Birds *birds;
};

#endif // DROUGHT_H
