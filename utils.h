#ifndef UTILS_H
#define UTILS_H
#include <QtMath>
#include <vector>

struct point {
    float x, y, z;
};

class Utils
{
public:

    static std::vector<float> getNormal(point p1, point p2, point p3);
};

#endif // UTILS_H
