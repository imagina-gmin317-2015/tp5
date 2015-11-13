#include "birds.h"

Birds::Birds(   int birdCount)
{
    PlyEntity *target = PlyEntity::load(":/gull.ply");

    //entities.push_back(target);

    for (int i = 0; i < birdCount; ++i) {
        PlyEntity *e = PlyEntity::copy(target);
        float x = (qrand() % 1000) * 0.001 - 0.5;
        float y = (qrand() % 1000) * 0.001 - 0.5;

        float z = (qrand() % 100) * 0.001 + 0.15;

        e->setPosition(x, y, z);
        e->setAngle(qrand() % 360);

        e->setScale(0.1f);

        entities.push_back(e);
    }
    elapsed = 0;
}

void Birds::update(float delta)
{
    elapsed += delta;
#pragma omp parallel for
    for (int i = 0; i < entities.size(); ++i) {
        int angle = elapsed * ((i % 2 == 0) ? 1 : -1) * (i % 8 + 5) * 50;
        float speed = 0.5;
        PlyEntity *e = entities[i];
        float x = cos(angle * 0.0174533f) * speed * delta;
        float y = sin(angle * 0.0174533f) * speed * delta;
        e->setAngle(angle + 90);
        e->setPosition(x + e->getX(), y + e->getY(), e->getZ());
    }
}

void Birds::draw(float delta)
{
//#pragma omp for schedule(dynamic)
    for (int i = 0; i < entities.size(); ++i) {
        entities[i]->draw(delta);
    }
}
