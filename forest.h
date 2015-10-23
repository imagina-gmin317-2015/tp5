#ifndef FOREST_H
#define FOREST_H

#include <QImage>
#include <QVector>

#include "plyentity.h"

class Forest
{
public:
    Forest(QImage *image, int treeCount);
    void update(float delta);
    void draw(float delta);

private:
    QImage *image;
    QVector<PlyEntity *> entities;
};

#endif // FOREST_H
