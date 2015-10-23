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
    float x, y, z, height;
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
    bool isActive;
    QVector<Grass*> grass;
    float windX, windY;
    QImage *image;
    float elapsed;
    float age;

    Grass *initGrass(Grass *g);
};

#endif // SPRING_H
