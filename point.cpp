#include "point.h"

Point::Point() :
    x(0),
    y(0),
    z(0)
{}

Point::Point(double x, double y, double z) :
    x(x),
    y(y),
    z(z)
{}

Point::Point(const Point &P)
{
    x = P.x;
    y = P.y;
    z = P.z;
}

Point::~Point() {
    // TODO Auto-generated destructor stub
}

double Point::getX() const {
    return this->x;
}

double Point::getY() const {
    return this->y;
}

double Point::getZ() const {
    return this->z;
}

void Point::setX(double x) {
    this->x = x;
}

void Point::setY(double y) {
    this->y = y;
}

void Point::setZ(double z) {
    this->z = z;
}

