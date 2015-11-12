#include "particules.h"

Particules::Particules(){

}

Particules::Particules(Point** p, int i){
    pointsMap = p;
    nbPointsBase = i;
    particuleSize = 1;
    particuleColor = (GLfloat*) malloc(sizeof(GLfloat) * 3);
    particuleColor[0] = 0.8f;
    particuleColor[1] = 0.8f;
    particuleColor[2] = 0.8f;
}

Point** Particules::getParticules(){
    return points;
}

void Particules::setParticules(Point** p){
    points = p;
}

Point** Particules::getPointsBase(){
    return pointsMap;
}

void Particules::setPointsBase(Point** p){
    pointsMap = p;
}

int Particules::getNbPointsBase(){
    return nbPointsBase;
}

void Particules::setNbPointsBase(int i){
    nbPointsBase = i;
}

void Particules::setNbParticules(int i){
    nbParticules = i;
}

int Particules::getNbParticules(){
    return nbParticules;
}

float* Particules::getSol(){
    return sol;
}

void Particules::setSol(float* s){
    sol = s;
}

int Particules::getParticuleSize(){
    return particuleSize;
}

void Particules::setParticuleSize(int i){
    particuleSize = i;
}

 GLfloat* Particules::getParticuleColor(){
     return particuleColor;
 }

void Particules::setParticuleColor(GLfloat* f){
    particuleColor = (GLfloat*) malloc(sizeof(GLfloat) * 3);
    particuleColor[0] = f[0];
    particuleColor[1] = f[1];
    particuleColor[2] = f[2];
}


void  Particules::init(GLfloat taux, GLfloat vitesseMin, GLfloat vitesseMax){
    nbParticules = nbPointsBase * taux;
    points = (Point**)malloc(sizeof(Point*) * nbParticules);
    sol = (float*)malloc(sizeof(float) * nbParticules);

    srand(time(NULL));
    for(int i = 0; i < nbParticules ; i++){
        points[i] = (Point*)malloc(sizeof(Point));
        int x = (qrand() % nbPointsBase);
        //qDebug()<<"Point "<<x<<" sur "<<nbPointsBase;

        float z = (qrand() % 100) * 0.02f;
        float v = (float) ((qrand() % (int)vitesseMax) + vitesseMin) / 1000.0f;//trop rapide sinon

        points[i] = new Point(pointsMap[x]->getX(),pointsMap[x]->getY(),z,v);
        sol[i] = pointsMap[x]->getZ();
    }
   // qDebug("Fin de l'init");
}

void Particules::update(){
    for(int i = 0; i < nbParticules; i++){
        points[i]->setZ(points[i]->getZ() - points[i]->getSpeed());

        //tester si on est sous le Z de la map        
        if(sol[i] > points[i]->getZ()) {
            int x = (qrand() % nbPointsBase);
            float z = (qrand() % 100) * 0.02f;
            float v = points[i]->getZ();
            points[i]->setX(pointsMap[x]->getX());
            points[i]->setY(pointsMap[x]->getY());
            points[i]->setZ(z);
            points[i]->setSpeed(v);
            sol[i] = pointsMap[x]->getZ();
        }
    }

    glPushMatrix();
    glPointSize(particuleSize);
    glBegin(GL_POINTS);
    glColor3f(particuleColor[0], particuleColor[1], particuleColor[2]);
    for(int i = 0; i < nbParticules; i++){
        glVertex3f(points[i]->getX(), points[i]->getY(), points[i]->getZ());
    }

    glEnd();
    glPopMatrix();
 }
