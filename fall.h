#ifndef FALL_H
#define FALL_H

#include <QObject>
#include <QTimer>
#include <QVector>
#include <QVector3D>
#include <QVector4D>
#include <QDebug>

#define MAX_PARTICULES 700
#define PARTICULE_MAX_Y 60

class Fall : public QObject
{
    Q_OBJECT
public:
    explicit Fall(QObject *parent = 0);
    Fall(QVector<QVector3D> particules, QVector<float> ground, QVector<float> fallSpeed, QTimer *timer = NULL);
    ~Fall();

    QVector<QVector3D> getParticules();
    QVector4D getPointColor();
    float getPointSize();
    void setPointColor(QVector4D);
    void setPointSize(float);
    void setFaster(float);

private:
    QVector<QVector3D> _particules;
    QVector<float> _grounds;
    QVector<float> _fallSpeed;
    QVector4D _pointColor;
    float _pointSize;
    float _faster;

signals:

public slots:
    void update();
};

#endif // FALL_H
