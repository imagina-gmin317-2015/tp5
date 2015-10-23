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

