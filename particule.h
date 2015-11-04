#ifndef PARTICULE_H
#define PARTICULE_H

#include <QObject>
#include <QTimer>

class Particule : public QObject
{
    Q_OBJECT
public:
    explicit Particule(QObject *parent = 0);
    Particule(float x = 0, float y = 0, float ground = 0, QTimer *timer = NULL);
    float getX();
    float getY();
    float getZ();

    ~Particule();
private:
    float x, y, z, ground;
    float moveSpeed;

signals:

public slots:
    void update();

};

#endif // PARTICULE_H
