#ifndef TERRAIN_H
#define TERRAIN_H

#include <QVector3D>
#include <QString>

#include "commonstruct.h"

class GameObject;

class Terrain
{
public:
    Terrain();
    Terrain(QString season, color c, int nb_v_w, int nb_v_h, point* p, GameObject*** t);
    Terrain(QString season, color c, int nb_v_w, int nb_v_h, QVector3D* v, GameObject*** t);

public:
    QString saison;
    color c;

    int nb_vertex_width;
    int nb_vertex_height;
    QVector3D* vertex;

    GameObject*** tree;
signals:

public slots:
};

#endif // TERRAIN_H
