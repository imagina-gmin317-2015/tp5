#ifndef FACE
#define FACE

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
class Face{
private:
    Point** points;
    int nbPoints;

public:
    Face();
    Face(int);
    Face(int, Point**);

    Point** getPoints();
    void setPoints(Point**);
    int getNbPoints();
    void setNbPoints(int);
};

#endif // FACE

