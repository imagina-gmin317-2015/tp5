#ifndef CAMERA_H
#define CAMERA_H

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>
#include <QVector>
#include <QDebug>
#include "resourcemanager.h"

class Camera
{
public:
    Camera();

    virtual void initialize(qreal ratio, qreal width, qreal height, qreal near, qreal far);
    void rotate(float x, float y, float z);
    void scale(float scaleX, float scaleY, float scaleZ);
    void translate(float x, float y, float z);
    void update(float delta);
    void setAnimated(bool b);

    float getRotationX() const;
    float getRotationY() const;
    float getRotationZ() const;

    float getScaleX() const;
    float getScaleY() const;
    float geScaleZ() const;

    bool isAnimated() const;

    float getX() const;
    float getY() const;

    void setCursorCaptured(bool b);
    bool isCursorCaptured() const;

    QVector<data *> attributes();

private:
    int etat = 0;
    float rotX = -45.0;
    float rotY = -45.0;
    float rotZ = .0;
    float ss = 3.0f;
    float x = 0.5;
    float y = 0.5;
    float z = 1;
    bool animated = false;
    bool cursorCaptured = false;
};

#endif // CAMERA_H
