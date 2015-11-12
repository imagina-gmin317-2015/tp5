#ifndef TRIANGLEWINDOW
#define TRIANGLEWINDOW



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

#include <QtCore>
#include <QtGui>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QThread>

#include "point.h"
#include "ply.h"
#include "particules.h"


class paramCamera
{
public:
    float rotX = -45.0;
    float rotY = -45.0;
    float ss = 1.0f;
    float anim = 0.0f;

    int etat = 1;
};


class TriangleWindow : public OpenGLWindow
{
public:
    TriangleWindow();
    TriangleWindow(int maj);
    TriangleWindow(bool);
    void initialize();
    void render();
    bool event(QEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void displayTriangles();
    void displayLines();
    void displayTrianglesC();
    void displayPoints();
    void displayTrianglesTexture();

    void displayColor(float);

    void loadMap(QString localPath);
    paramCamera* c;

    //TP3
    void displaySeasons();
    void setSeason(int,bool);

    //TP4
    paramCamera *getCamera();
    int getSeason();
    Point** getPoints();
    void setPoints(Point**);
    int getNbPoints();
    void setNbPoints(int);
    int getWidthPoint();
    void setWidthPoint(int);
    int getHeightPoint();
    void setHeightPoint(int);

    Particules* getParticules();
    void setParticules(Particules*);

    Ply** getPolygones();
    void setPolygones(Ply**);
    int getNbPolygones();
    void setNbPolygones(int);
    void addPolygone(Ply*);
    void displayPolygones(int);

    void setColors(GLfloat**);
    GLfloat** getColors();


private:
    bool master = false;
    int m_frame;
    QImage m_image;
    int widthPoint;
    int heightPoint;
    int nbPoints;
    Point **p;


    int carte=0;


    int maj = 20;

    QTimer *timer;


    //Tp3
    int season = 0;
    QTimer *timerFiveMinutes;
    QTcpServer *server;
    QTcpSocket *socket;

    std::vector<QTcpSocket*> sockets;

    Particules *particules;

    //TP4
    Ply** polygones;
    int nbPolygones;

    GLfloat** colors;


public slots:
    void newConnection();
    void changeSeason();
};


#endif // TRIANGLEWINDOW

