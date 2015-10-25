#ifndef POINT_H
#define POINT_H

#include <iostream>

using namespace std;

class Point
{
    public:
        Point() {set(0,0,0);};
        Point(double x, double y, double z) {set(x,y,z);};
        Point(const Point& p) {set(p.getX(),p.getY(), p.getZ());};

        double getX() const {return x;};
        double getY() const {return y;};
        double getZ() const {return z;};

        void set(double x, double y, double z) {setX(x); setY(y); setZ(z);};
        void setX(double _x) {x = _x;};
        void setY(double _y) {y = _y;};
        void setZ(double _z) {z = _z;};

        //Méthode d'affichage en console d'un Point
        void affiche() const { cout << "Point(" << x << "," << y << "," << z << ")" << endl; };

        friend Point operator+(const Point& p1, const Point& p2)
        { return Point(p1.getX()+p2.getX(), p1.getY()+p2.getY(), p1.getZ()+p2.getZ()); }
    private:
        double x, y, z;
};

#endif // POINT_H
