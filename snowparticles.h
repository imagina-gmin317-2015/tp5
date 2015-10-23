#ifndef SNOWPARTICLES_H
#define SNOWPARTICLES_H

#include <QImage>
#include <QVector>
#include <QRgb>
#include <QtGui/QOpenGLFunctions>
#include <iostream>
#include <vector>

#include <utils.h>
#include "pool.cpp"
#include "resourcemanager.h"

class SnowFlake {
    friend class Pool<SnowFlake *>;

public:
    float x, y, z, speed;
    static Pool<SnowFlake *> *pool;
    SnowFlake *init(float x, float y, float z);
private:
    SnowFlake();
};

class SnowParticles
{
public:
    SnowParticles(int width, int height, QImage *image);
    void update(float delta);
    void draw(float delta);
    void reset();
    void setActive(bool active);

    QVector<data *> attributes();

 private:
    int width;
    int height;
    float **snow;
    QImage *image;
    QVector<SnowFlake*> snowFlakes;
    bool isActive;

    SnowFlake *createSnowFlake(SnowFlake *s);
};

#endif // SNOWPARTICLES_H
