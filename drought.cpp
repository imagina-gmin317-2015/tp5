#include "drought.h"

Drought::Drought()
{
    yellow = 0;
    snowHeight = 0;
    birds = new Birds(10);
}

void Drought::update(float delta)
{
    if(isActive) {
        if(yellow < 0.3f) yellow += 0.02f * delta;
        if(snowHeight < 0.05f) snowHeight += 0.002f * delta;
        birds->update(delta);
    } else {
        if(yellow > 0) yellow -= 0.02f * delta;
        if(snowHeight > 0) snowHeight -= 0.004f * delta;
    }
}

void Drought::draw()
{
    if(isActive)
        birds->draw(0);
}

float Drought::getYellow() const
{
    return yellow;
}

float Drought::getSnowHeightModifier() const
{
    return snowHeight;
}

void Drought::setActive(bool active)
{
    this->isActive = active;
}

bool Drought::active() const
{
    return this->isActive;
}

QVector<data *> Drought::attributes()
{
    QVector<data *> att;
    att.push_back(ResourceManager::FLOAT(&yellow));
    att.push_back(ResourceManager::FLOAT(&snowHeight));
    att.push_back(ResourceManager::BOOL(&isActive));
    return att;
}
