#ifndef PLYENTITY_H
#define PLYENTITY_H

#include <QVector>
#include <QString>
#include <QtCore>
#include <QtGui>
#include <QFile>
#include <QDebug>
#include <QtGui/QOpenGLFunctions>

#include <iostream>

#include "utils.h"

class PlyEntity
{
public:
    static PlyEntity *load(QString filePath);
    static PlyEntity *copy(PlyEntity *entity);
    void draw(float delta);
    void update(float delta);
    void setPosition(float x, float y, float z);
    void setScale(float scale);
    void setAngle(float angle);

    float getX() const;
    float getY() const;
    float getZ() const;
    float getAngle() const;

protected:
    PlyEntity(QStringList list);
    PlyEntity();

private:
    QVector<point> vertices;
    QVector<QVector<int>> indexes;
    float x, y, z, scale, angle;

    QVector<QVector3D> verticesArray, scaledVerticesArray;
    QVector<QVector3D> normalsArray;
    QVector<QVector3D> colorsArray;
};

#endif // PLYENTITY_H
