#include "point.h"

Point::Point(){
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

Point::Point(float X, float Y, float Z){
    x = X;
    y = Y;
    z = Z;
}

Point::Point(float X, float Y, float Z, float S){
    x = X;
    y = Y;
    z = Z;
    speed = S;
}



void Point::setX(float X){
    x = X;
}

void Point::setY(float Y){
    y = Y;
}

void Point::setZ(float Z){
    z = Z;
}


float Point::getX(){
    return x;
}

float Point::getY(){
    return y;
}

float Point::getZ(){
    return z;
}


void Point::setSpeed(float s){
    speed = s;
}

float Point::getSpeed(){
    return speed;
}


void Point::setCouleur(float * c){
    couleur = c;
}

float* Point::getCouleur(){
    return couleur;
}

