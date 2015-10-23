#include "spring.h"
Pool<Grass *> *Grass::pool = new Pool<Grass*>([] () {return new Grass();});

Spring::Spring(QImage *image)
{
    this->image = image;
    this->elapsed = 0;
    this->age = 0;
}

void Spring::update(float delta)
{
    this->elapsed += delta * 10;
    this->windX = cos(elapsed) * 0.001;
    //    this->windY = cos(elapsed) * 0.01;
    this->windY = 0;
    if(isActive) {
        if(grass.size() < 700 && qrand() % 100 < 20) {
            #pragma omp for schedule(dynamic)
            for (int i = 0; i < 2; ++i) {
                float x = qrand() % this->image->width();
                float y = qrand() % this->image->height();
                float z = qGray(this->image->pixel(x, y));
                if(z * 0.0008 < 0.07) {
                    grass.push_back(Grass::pool->obtain()->init(x / (float) this->image->width() - 0.5, y  / (float) this->image->height() - 0.5, z * 0.0008));
                }
            }
        }
        #pragma omp parallel for
        for (int i = 0; i < grass.size(); ++i) {
            if(grass[i]->height < 0.01) grass[i]->height += 0.0001;
        }
    } else {
        if(age < 0.5) {
            age += delta * 0.1;
        } else {
            bool found = false;
            #pragma omp parallel for
            for (int i = 0; i < grass.size(); ++i) {
                if(grass[i]->height > - 0.008) {
                    found = true;
                    grass[i]->height -= delta * 0.01;
                }
            }
            if(!found) {
                grass.clear();
            }
        }
    }
}

void Spring::draw(float delta)
{
    #pragma omp for schedule(dynamic)
    for (int i = 0; i < grass.size(); ++i) {
        glColor3f(0.01, 0.6, 0.0);
        glBegin(GL_LINES);
        glVertex3f(grass[i]->x, grass[i]->y, grass[i]->z);
        glVertex3f(grass[i]->x + windX, grass[i]->y + windY, grass[i]->z+grass[i]->height);
        glEnd();
        glColor3f(1 - age, age, 0);
        glBegin(GL_TRIANGLES);
        glVertex3f(grass[i]->x + windX, grass[i]->y + windY, grass[i]->z + grass[i]->height);
        glVertex3f(grass[i]->x + windX + 0.002, grass[i]->y + windY, grass[i]->z + grass[i]->height + 0.005);
        glVertex3f(grass[i]->x + windX - 0.002, grass[i]->y + windY, grass[i]->z + grass[i]->height + 0.005);

        glVertex3f(grass[i]->x + windX + 0.002, grass[i]->y + windY, grass[i]->z + grass[i]->height + 0.005);
        glVertex3f(grass[i]->x + windX, grass[i]->y + windY, grass[i]->z + grass[i]->height);
        glVertex3f(grass[i]->x + windX - 0.002, grass[i]->y + windY, grass[i]->z + grass[i]->height + 0.005);

        glVertex3f(grass[i]->x + windX, grass[i]->y + windY, grass[i]->z + grass[i]->height);
        glVertex3f(grass[i]->x + windX, grass[i]->y + windY + 0.002, grass[i]->z + grass[i]->height + 0.005);
        glVertex3f(grass[i]->x + windX, grass[i]->y + windY - 0.002, grass[i]->z + grass[i]->height + 0.005);

        glVertex3f(grass[i]->x + windX, grass[i]->y + windY + 0.002, grass[i]->z + grass[i]->height + 0.005);
        glVertex3f(grass[i]->x + windX, grass[i]->y + windY, grass[i]->z + grass[i]->height);
        glVertex3f(grass[i]->x + windX, grass[i]->y + windY - 0.002, grass[i]->z + grass[i]->height + 0.005);
        glEnd();
    }
}

void Spring::setActive(bool active)
{
    if(active) this->age = 0;
    this->isActive = active;
}

QVector<data *> Spring::attributes()
{
    QVector<data *> att;
    att.push_back(ResourceManager::FLOAT(&elapsed));
    att.push_back(ResourceManager::FLOAT(&age));
    att.push_back(ResourceManager::BOOL(&isActive));
    return att;
}

Grass *Spring::initGrass(Grass *g)
{

}


Grass *Grass::init(float x, float y, float z)
{
    this->x = x; this->y = y; this->z = z;
    return this;
}

Grass::Grass()
{

}
