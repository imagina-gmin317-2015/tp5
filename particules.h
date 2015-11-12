#ifndef PARTICULES
#define PARTICULES

#include <QtCore>

#include "openglwindow.h"
#include "point.h"
class Particules{
public:
    Particules();
    Particules(Point**, int);

    //taux entre 0 et 1 pour connaitre le nombre de particules à afficher par
    //rapport aux nombres de points représentant la map
    void init(GLfloat taux, GLfloat vitesseMin, GLfloat vitesseMax);

    Point** getParticules();
    void setParticules(Point**);

    Point** getPointsBase();
    void setPointsBase(Point**);

    int getNbPointsBase();
    void setNbPointsBase(int);

    void setNbParticules(int);
    int getNbParticules();

    float* getSol();
    void setSol(float*);



    void update();

    void setParticuleSize(int);
    int getParticuleSize();
    void setParticuleColor(GLfloat*);
    GLfloat* getParticuleColor();

private :
    Point** pointsMap;//les points de base
    int nbPointsBase;
    Point **points; //le tableau de points
    float* sol;//les coordonnées de disparition des particules
    int nbParticules; //le nombre de points/particules affichées


    int particuleSize; //taille des particules
    GLfloat *particuleColor; //couleur des particules
};

#endif // PARTICULES

