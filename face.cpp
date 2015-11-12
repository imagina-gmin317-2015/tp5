#include "face.h"

Face::Face(){

}

Face::Face(int n){
    nbPoints = n;
    points = (Point**)malloc(sizeof(Point*) * nbPoints);
    for(int i = 0; i < nbPoints; i++){
        points[i] = new Point();
    }
}

Face::Face(int n, Point** p){
    nbPoints = n;
    points = p;
}

Point** Face::getPoints(){
    return points;
}

void Face::setPoints(Point**p){
    points = p;
}

int Face::getNbPoints(){
    return nbPoints;
}

void Face::setNbPoints(int n){
    nbPoints = n;
}
