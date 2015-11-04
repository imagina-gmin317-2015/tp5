#ifndef RAINPARTICLES_H
#define RAINPARTICLES_H

#include <QImage>
#include <QVector>
#include <QRgb>
#include <QtGui/QOpenGLFunctions>
#include <iostream>
#include <QtGui/QOpenGLShaderProgram>
#include <vector>
#include <QOpenGLBuffer>
#include <QVector2D>

#include "pool.cpp"
#include "utils.h"
#include "resourcemanager.h"
#include "galleon.h"

class RainDrop {
    friend class Pool<RainDrop *>;

public:
    float x, y, z, speed;
    static Pool<RainDrop *> *pool;
    RainDrop *init(float x, float y, float z);
private:
    RainDrop();
};

class RainParticles
{
public:
    RainParticles(QImage *image);
    void update(float delta);
    void draw(float delta);
    void reset();
    void setActive(bool active);

    QVector<data *> attributes();

private:
    int width;
    int height;
    float waterHeight;
    QImage *image;
    QVector<RainDrop*> rainDrops;
    float elapsed;
    bool isActive;
    float lightningX, lightningY;
    Galleon *galleon;

    QOpenGLShaderProgram *shader;
    QVector<QVector2D> waterArray;
    QOpenGLBuffer waterBuffer;

    RainDrop *createRainDrops(RainDrop *s);
    void initWater();
};

#endif // RAINPARTICLES_H
