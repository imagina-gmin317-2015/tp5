#include "forest.h"

Forest::Forest(QImage *image, int treeCount)
{
    this->image = image;
    PlyEntity *target = PlyEntity::load(":/c_autumntree.ply");

    //entities.push_back(target);

    for (int i = 0; i < treeCount; ++i) {
        PlyEntity *e = PlyEntity::copy(target);
        float x = (qrand() % 1000) * 0.001 - 0.5;
        float y = (qrand() % 1000) * 0.001 - 0.5;

        int x1 = (x + 0.5) * this->image->width();
        int y1 = (y + 0.5) * this->image->height();

        float z = qGray(this->image->pixel(x1, y1)) * 0.0008;

        e->setPosition(x, y, z);
        e->setAngle(qrand() % 360);

        e->setScale(0.01f + (qrand() % 100) * 0.0001);

        entities.push_back(e);
    }

}

void Forest::update(float delta)
{

}

void Forest::draw(float delta)
{
//#pragma omp for schedule(dynamic)
    for (int i = 0; i < entities.size(); ++i) {
        entities[i]->draw(delta);
    }
}
