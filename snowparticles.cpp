#include "snowparticles.h"
Pool<SnowFlake *> *SnowFlake::pool = new Pool<SnowFlake*>([] () {return new SnowFlake();});

SnowParticles::SnowParticles(int width, int height, QImage *image)
{
    this->width = image->width();
    this->height = image->height();
    snow = new float*[width];
    for (int i = 0; i < width; ++i) {
        snow[i] = new float[height];
    }
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            snow[i][j] = 0;
        }
    }
    this->image = image;

    //        for (int i = 0; i < 500; ++i) {
    //            snowFlakes.push_back(createSnowFlake(SnowFlake::pool->obtain()));
    //        }

}

void SnowParticles::update(float delta)
{
#pragma omp parallel for
    for (int i = 0; i < snowFlakes.size(); ++i) {
        SnowFlake *s = snowFlakes[i];
        s->z -= s->speed * delta;
        int x = s->x * image->width() + image->width() * 0.5;
        int y = s->y * image->width() + image->width() * 0.5;
        float value = qGray(image->pixel(x, y)) * 0.0008f;
        if(value > s->z) {
            snow[x][y] += 0.001;
            createSnowFlake(s);
        }
    }
    if(isActive) {
        if(snowFlakes.size() < 700 && qrand() % 100 < 50) {
            snowFlakes.push_back(createSnowFlake(SnowFlake::pool->obtain()));
        }
    } else {
#pragma omp for schedule(dynamic)
        for (int i = 0; i < 5; ++i) {
            if(snowFlakes.begin() != snowFlakes.end()) {
                SnowFlake *r = snowFlakes[0];
                SnowFlake::pool->release(r);
                snowFlakes.erase(snowFlakes.begin());
            }
        }
#pragma omp parallel for
        for (int i = 0; i < width; ++i) {
#pragma omp parallel for
            for (int j = 0; j < height; ++j) {
                if(snow[i][j] > 0 && qrand() % 100 < 2)
                    snow[i][j] -= 0.001;
            }
        }
    }

}

void SnowParticles::draw(float delta)
{
    glPointSize(3);
    glBegin(GL_POINTS);
    glColor3f(0.9, 0.8, 0.9);
#pragma omp for schedule(dynamic)
    for (int i = 0; i < snowFlakes.size(); ++i) {
        SnowFlake *s = snowFlakes[i];
        //        int x = s->x * this->image->width();
        //        int y = s->y * this->image->height();
        glVertex3f(s->x, s->y, s->z);
    }

    //    glBegin();
#pragma omp for schedule(dynamic)
    for (int i = 0; i < image->width(); ++i) {
        for (int j = 0; j < image->height(); ++j) {
            if(snow[i][j] > 0) {
                glVertex3f((float) i / (float)image->width() - 0.5f,
                           (float) j / (float)image->height() - 0.5f,
                           qGray(image->pixel(i, j)) * 0.0008f + snow[i][j]);
                //                point p1, p2, p3;
                //                p1.x = i; p1.y = j; p1.z = qGray(image->pixel(i, j));
                //                p2.x = i + 1; p2.y = j; p2.z = qGray(image->pixel(i + 1, j));
                //                p3.x = i; p3.y = j + 1; p3.z = qGray(image->pixel(i, j + 1));
                //                std::vector<float> vec = Utils::getNormal(p1, p3, p2);
                //                glNormal3f(vec[0], vec[1], vec[2]);
            }

        }
    }
    glEnd();
}

void SnowParticles::reset()
{
    //#pragma omp for schedule(dynamic)
    //    for (int i = 0; i < snowFlakes.size(); ++i) {
    //        SnowFlake::pool->release(snowFlakes[i]);
    //    }
    //    snowFlakes.clear();
}

void SnowParticles::setActive(bool active)
{
    this->isActive = active;
}

QVector<data *> SnowParticles::attributes()
{
    QVector<data *> att;
    att.push_back(ResourceManager::INT(&width));
    att.push_back(ResourceManager::INT(&height));
    att.push_back(ResourceManager::BOOL(&isActive));
    return att;
}

SnowFlake *SnowParticles::createSnowFlake(SnowFlake *s)
{
    s->x = (float) ((qrand() % width) - width * 0.5) / ((float) width);
    s->y = (float) ((qrand() % height) - height * 0.5) / (float) height;
    s->z = (qrand() % 100) * 0.01f;
    return s;
}

SnowFlake *SnowFlake::init(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
    return this;
}

SnowFlake::SnowFlake()
{
    this->speed = (float (qrand() % 10) + 5.0f) * 0.06f;
}
