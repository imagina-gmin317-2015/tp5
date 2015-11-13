#include "galleon.h"

Galleon::Galleon(QImage *image)
{
    this->entity = PlyEntity::load(":/galleon.ply");
    this->entity->setScale(0.0001f);
    this->speed = 0.05f;
    this->angle = qrand() % 360;
    this->image = image;
    this->elapsed = 0;
    this->animated = false;
    this->rotation = 1;
}

void Galleon::update(float delta)
{
    elapsed += delta;
    if(qrand() % 1000 < 5) rotation *= -1;

//    int x1 = 0; int y1 = 0;

    int x1 = (this->entity->getX() + 0.5) * this->image->width();
    int y1 = (this->entity->getY() + 0.5) * this->image->height();

    if(x1 > this->image->width()) x1 = this->image->width() - 1;
    if(y1 > this->image->height()) y1 = this->image->height() - 1;

    if(waterHeight - qGray(this->image->pixel(x1, y1)) * 0.0008 > 0.01) {
        this->animated = true;
        float x = cos(angle * 0.0174533f) * speed * delta;
        float y = sin(angle * 0.0174533f) * speed * delta;

        if(entity->getX() < -0.5 || entity->getY() < -0.5 || entity->getX() > 0.5 || entity->getY() > 0.5 || waterHeight - qGray(this->image->pixel(x1, y1)) * 0.0008 < 0.02) {
            this->angle += 5 * rotation;
            if(angle > 360) angle = 0;
        }
        this->entity->setAngle(angle + 90);

        this->entity->setPosition(x + this->entity->getX(), y + this->entity->getY(), waterHeight);
    } else {
        this->animated = false;
    }
}

void Galleon::draw()
{
    if(this->animated) {
        glPushMatrix();
        glRotatef(sin(elapsed * 10), 1, 0, 0);
        glRotatef(cos(elapsed * 10), 0, 1, 0);
    }
    entity->draw(0);
    if(this->animated) {
        glPopMatrix();
    }
}

void Galleon::setWaterHeight(float waterHeight)
{
    this->waterHeight = waterHeight;
}
