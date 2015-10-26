#ifndef UTILS_H
#define UTILS_H
#include <QtMath>
#include <vector>
#include <QVector>
#include <QVector3D>

struct point {
    float x, y, z;
};

class Utils
{
public:

    static std::vector<float> getNormal(point p1, point p2, point p3);
    static QVector3D getNormal(QVector3D v1, QVector3D v2, QVector3D v3);
};

#endif // UTILS_H
