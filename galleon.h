#ifndef GALLEON_H
#define GALLEON_H

#include <QImage>

#include "plyentity.h"

class Galleon
{
public:
    Galleon(QImage *image);
    void update(float delta);
    void draw();
    void setWaterHeight(float waterHeight);

private:
    PlyEntity *entity;
    float speed;
    float angle;
    float waterHeight;
    QImage *image;
    float elapsed;
    bool animated;
    int rotation;
};

#endif // GALLEON_H
