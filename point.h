#ifndef POINT
#define POINT

class Point{
public:
    Point();
    Point(float,float,float);
    Point(float,float,float,float);

    void setX(float);
    void setY(float);
    void setZ(float);

    float getX();
    float getY();
    float getZ();

    void setSpeed(float);
    float getSpeed();

    void setCouleur(float*);
    float* getCouleur();

private:
    float x;
    float y;
    float z;

    float speed;
    float * couleur;

};

#endif // POINT

