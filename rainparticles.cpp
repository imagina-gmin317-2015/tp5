#include "rainparticles.h"
Pool<RainDrop *> *RainDrop::pool = new Pool<RainDrop*>([] () {return new RainDrop();});

RainParticles::RainParticles(QImage *image)
{
    this->width = image->width();
    this->height = image->height();
    this->image = image;
    this->waterHeight = 0.0;
    this->elapsed = 0;
    this->isActive = false;
    this->galleon = new Galleon(image);
}

void RainParticles::update(float delta)
{
    float f = 0.1;

    if(lightningX == -2 && qrand() % 1000 < 100) {
        lightningX = ((qrand() % 100) - 50) * 0.01;
        lightningY = ((qrand() % 100) - 50) * 0.01;
    }
    if(qrand() % 1000 > 800) {
        lightningX = -2;
        lightningY = -2;
    }

    this->galleon->setWaterHeight(waterHeight);
    this->galleon->update(delta);

#pragma omp parallel for
    for (int i = 0; i < rainDrops.size(); ++i) {
        RainDrop *s = rainDrops[i];
        s->z -= s->speed * delta;
        int x = s->x * image->width() + image->width() * 0.5;
        int y = s->y * image->width() + image->width() * 0.5;
        float value = qGray(image->pixel(x, y)) * 0.0008f;
        if(value > s->z) {
            createRainDrops(s);
            if(waterHeight < 0.1) waterHeight += 0.0002f * delta;
        }
    }

    if(isActive) {
        if(rainDrops.size() < 1000 && qrand() % 100 < 25) {
            rainDrops.push_back(createRainDrops(RainDrop::pool->obtain()));
        }
    } else {
#pragma omp for schedule(dynamic)
        for (int i = 0; i < 3; ++i) {
            if(rainDrops.begin() != rainDrops.end()) {
                RainDrop *r = rainDrops[0];
                RainDrop::pool->release(r);
                rainDrops.erase(rainDrops.begin());
            }
        }
        if(waterHeight > 0.001)
            waterHeight -= 0.01 * delta;
    }
}

void RainParticles::draw(float delta)
{

    elapsed += delta * 10;
    glPointSize(1);
    glBegin(GL_LINES);
    glColor4f(0.2, 0.2, 1, 1);
#pragma omp for schedule(dynamic)
    for (int i = 0; i < rainDrops.size(); ++i) {
        RainDrop *s = rainDrops[i];
        glVertex3f(s->x, s->y, s->z);
        glVertex3f(s->x, s->y, s->z + 0.01);
    }
    glEnd();

    if(isActive) {
        if(lightningX > -2) {

            glEnable(GL_LIGHT1);
            float LightPos[4] = {lightningX, lightningY,0.7,1};
            glLightfv(GL_LIGHT1,GL_POSITION,LightPos);
            float LightDif[4] = {1,1,1.f,1.f};
            glLightfv(GL_LIGHT1,GL_DIFFUSE,LightDif);

            //            glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 2.0);
            //            glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1.0);
            //            glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 3);

            glColor3f(1, 1, 1);
            glBegin(GL_LINE_STRIP);
            float f1, f2, f3;
            f1 = ((qrand() % 10) - 5) * 0.1;
            f2 = ((qrand() % 10) - 5) * 0.01;
            f3 = ((qrand() % 10) - 5) * 0.01;
            glVertex3f(lightningX + f1, lightningY + f1, 10);
            glVertex3f(lightningX + f3, lightningY + f2, 0.7);
            glVertex3f(lightningX + f2, lightningY, 0.3);
            glVertex3f(lightningX, lightningY, 0);
            //    glVertex3f(0.5, 0.5, 1);
            glEnd();
        } else {
            glDisable(GL_LIGHT1);
        }
        this->galleon->draw();
    }

    if(waterHeight > 0.0001) {
        glColor4f(0.2, 0.2, 1, 0.5);
        glBegin(GL_QUADS);
#pragma omp for schedule(dynamic)
        for (int i = -50; i < 50; ++i) {
            for (int j = -50; j < 50; ++j) {
                float r = 0;
                point p1, p2, p3, p4;
                p1.x = i * 0.01; p1.y = j * 0.01;
                p1.z = waterHeight + sin(i + elapsed + r) * 0.005 + cos(j + elapsed + r) * 0.005;

                p2.x = (i + 1) * 0.01; p2.y = j * 0.01;
                p2.z = waterHeight + sin(i + 1 + elapsed + r) * 0.005 + cos(j + elapsed + r) * 0.005;

                p3.x = (i + 1) * 0.01; p3.y = (j + 1) * 0.01;
                p3.z = waterHeight + sin(i + 1 + elapsed + r) * 0.005 + cos(j + 1 + elapsed + r) * 0.005;

                p4.x = i * 0.01; p4.y = (j + 1) * 0.01;
                p4.z = waterHeight + sin(i + elapsed + r) * 0.005 + cos(j + 1 + elapsed + r) * 0.005;

                float z = qGray(image->pixel((i + 50) * this->image->width() * 0.01, (j + 50) * this->image->height() * 0.01));
                z *= 0.0008;
                //                qDebug() << (i + 50) * 240 * 0.01;
                std::vector<float> vec = Utils::getNormal(p1, p3, p2);
                glNormal3f(vec[0], vec[1], vec[2]);
                if(p1.z > z || p2.z > z || p3.z > z) {
                    glVertex3f(p1.x, p1.y, p1.z);
                    glVertex3f(p2.x, p2.y, p2.z);
                    glVertex3f(p3.x, p3.y, p3.z);
                    glVertex3f(p4.x, p4.y, p4.z);
                }

                if( z < waterHeight) {
                    if(i == -50) {
                        glVertex3f(-0.5, p3.y, z);
                        glVertex3f(-0.5, p1.y, z);
                        glVertex3f(-0.5, p1.y, p1.z);
                        glVertex3f(-0.5, p3.y, p4.z);
                    } else if (i == 49) {
                        glVertex3f(0.5, p3.y, p3.z);
                        glVertex3f(0.5, p1.y, p2.z);
                        glVertex3f(0.5, p1.y, z);
                        glVertex3f(0.5, p3.y, z);
                    }
                    if(j == -50) {
                        glVertex3f(p1.x, -0.5, z);
                        glVertex3f(p3.x, -0.5, z);
                        glVertex3f(p3.x, -0.5, p2.z);
                        glVertex3f(p1.x, -0.5, p1.z);
                    } else if(j == 49) {
                        glVertex3f(p1.x, 0.5, p4.z);
                        glVertex3f(p3.x, 0.5, p3.z);
                        glVertex3f(p3.x, 0.5, z);
                        glVertex3f(p1.x, 0.5, z);
                    }
                }
            }
        }
        glEnd();
    }
}

void RainParticles::reset()
{
    //    for (int i = 0; i < rainDrops.size(); ++i) {
    //        RainDrop::pool->release(rainDrops[i]);
    //    }
    //    rainDrops.clear();
    //    waterHeight = 0;
}

void RainParticles::setActive(bool active)
{
    this->isActive = active;
}

QVector<data *> RainParticles::attributes()
{
    QVector<data *> att;
    att.push_back(ResourceManager::FLOAT(&waterHeight));
    att.push_back(ResourceManager::FLOAT(&elapsed));
    att.push_back(ResourceManager::BOOL(&isActive));
    return att;
}

RainDrop *RainParticles::createRainDrops(RainDrop *s)
{
    s->x = (float) ((qrand() % width) - width * 0.5) / ((float) width);
    s->y = (float) ((qrand() % height) - height * 0.5) / (float) height;
    s->z = (qrand() % 100) * 0.01f;
    return s;
}

RainDrop *RainDrop::init(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
    return this;
}

RainDrop::RainDrop()
{
    this->speed = (float (qrand() % 10) + 50.0f) * 0.06f;
}
