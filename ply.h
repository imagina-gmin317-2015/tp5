#ifndef PLY
#define PLY


#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>

#include <QtCore/qmath.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <string>

#include <QtCore>
#include <QtGui>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QThread>

#include "face.h"

class Ply{
public:
    Ply();
    Ply(QString);

    void setPoints(Point**);
    Point** getPoints();
    void setNbPoints(int);
    int getNbPoints();
    void setNormales(Point**);
    Point** getNormales();
    void setFaces(Face**);
    Face** getFaces();
    void setNbFaces(int);
    int getNbFaces();
    void setTaille(float);
    float getTaille();
    void setPosX(float);
    float getPosX();
    void setPosY(float);
    float getPosY();
    void setPosZ(float);
    float getPosZ();
    void setRotX(float);
    float getRotX();
    void setRotY(float);
    float getRotY();
    void setRotZ(float);
    float getRotZ();
    QString getNom();
    void setNom(QString);

    float moyX();
    float moyY();
    float moyZ();

    void drawPoints();
    void drawFaces();
    void drawLines();

    void initMatrix();
    void delMatrix();
    void changeModel(QString);

private:
    QString nom;
    Point** points;
    int nbPoints;
    Point** normales;
    Face** faces;
    int nbFaces;

    float taille;
    float posX;
    float posY;
    float posZ;
    float rotX;
    float rotY;
    float rotZ;
};

#endif // PLY

