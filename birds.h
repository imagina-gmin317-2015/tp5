#ifndef BIRDS_H
#define BIRDS_H

#include <QVector>

#include "plyentity.h"

class Birds
{
public:
    Birds(int birdCount);
    void update(float delta);
    void draw(float delta);

private:
    QVector<PlyEntity *> entities;
    float elapsed;
};

#endif // BIRDS_H
