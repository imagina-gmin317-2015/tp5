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

    shader = new QOpenGLShaderProgram();
    shader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/water_vertex.glsl");
    shader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/water_fragment.glsl");
    qDebug() << "linked = " << shader->link();

    texture = new QOpenGLTexture(QImage(":/water.jpg"));
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    initWater();
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
            float zero[4] = {0, 0, 0, 0};
            glLightfv(GL_LIGHT1,GL_DIFFUSE,zero);
            glDisable(GL_LIGHT1);
        }
        this->galleon->draw();
    }

    if(waterHeight > 0.000001) {
        shader->bind();
        shader->setUniformValue("waterHeight", waterHeight);
        shader->setUniformValue("time", elapsed);
        texture->bind();
        glEnableClientState(GL_VERTEX_ARRAY);
        waterBuffer.bind();
        glVertexPointer(2, GL_FLOAT, 0, NULL);
        waterBuffer.release();
        glDrawArrays(GL_QUADS, 0, waterArray.size());
        glDisableClientState(GL_VERTEX_ARRAY);
        texture->release();
        shader->release();
    }
}

void RainParticles::reset()
{
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

void RainParticles::initWater()
{
    float n = 0.001;
    for (int i = -500; i < 500; ++i) {
        for (int j = -500; j < 500; ++j) {
            waterArray.push_back(QVector2D(i * n, (j + 1) * n));
            waterArray.push_back(QVector2D(i * n, j * n));
            waterArray.push_back(QVector2D((i + 1) * n, j * n));
            waterArray.push_back(QVector2D((i + 1) * n, (j + 1) * n));
        }
    }
    waterBuffer.create();
    waterBuffer.bind();
    waterBuffer.allocate(waterArray.constData(), waterArray.size() * sizeof(QVector2D));
    waterBuffer.release();
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
