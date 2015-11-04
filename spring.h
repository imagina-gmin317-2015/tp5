#ifndef SPRING_H
#define SPRING_H

#include <QtGui/QOpenGLFunctions>
#include <QVector>
#include <QImage>
#include <resourcemanager.h>

#include "pool.cpp"

class Grass {
    friend class Pool<Grass *>;

public:
    float x = 0, y = 0, z = 0, height = 0;
    static Pool<Grass *> *pool;
    Grass *init(float x, float y, float z);


private:
    Grass();
};

class Spring
{
public:
    Spring(QImage *image);
    void update(float delta);
    void draw(float delta);
    void setActive(bool active);

    QVector<data *> attributes();

private:
    bool isActive = false;
    QVector<Grass*> grass;
    float windX = 0, windY = 0;
    QImage *image;
    float elapsed = 0;
    float age = 0;

    Grass *initGrass(Grass *g);
};

#endif // SPRING_H
