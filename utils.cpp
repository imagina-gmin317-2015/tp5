#include "utils.h"

std::vector<float> Utils::getNormal(point t1, point t2, point t3) {
    point v, w, n;
    v.x = t2.x - t1.x; v.y = t2.y - t1.y; v.z = t2.z- t1.z;
    w.x = t3.x - t1.x; w.y = t3.y - t1.y; w.z = t3.z- t1.z;

    n.x = v.y * w.z - v.z * w.y;
    n.y = v.z * w.x - v.x * w.z;
    n.z = v.x * w.y - v.y * w.x;
    float l = sqrt(pow(n.x, 2)) + sqrt(pow(n.y, 2)) + sqrt(pow(n.z, 2));
    if(l == 0) l = 1;
    std::vector<float> res = {n.x / l, n.y / l, -n.z / l};
    return res;
}

QVector3D Utils::getNormal(QVector3D v1, QVector3D v2, QVector3D v3)
{
    QVector3D p1, p2, p3;
    if(v1.y() == v2.y()) {
        if(v1.x() < v3.x()) {
            p1 = v1; p2 = v2; p3 = v3;
        } else {
            p1 = v1; p2 = v3; p3 = v2;
        }
    } else {
        if(v1.x() < v3.x()) {
            p1 = v1; p2 = v3; p3 = v2;
        } else {
            p1 = v1; p2 = v2; p3 = v3;
        }
    }

    v1 = p2 - p1;
    v2 = p3 - p1;
    return QVector3D::normal(v1, v2);
}

