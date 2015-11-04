#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H
#include <QtGui/QGuiApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include "openglwindow.h"
#include "camera.h"
#include "point.h"
#include "particule.h"
#include "filemanager.h"

#include <time.h>

#include <QTcpSocket>
#include <QTimer>
#include <math.h>

#define MAX_PARTICULES 700
#define PI 3.141592653589793238463

class GameWindow : public OpenGLWindow
{
    Q_OBJECT
public:
    GameWindow(quint16 port);
    GameWindow(int maj, quint16 port);
    void initialize();
    void render();
    bool event(QEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void displayTriangles();
    void displayLines();
    void displayTrianglesC();
    void displayPoints();
    void displayTrianglesTexture();
    void drawParticules();

    void displayColor(float);

    void loadMap(QString localPath);
    void connectToServer(quint16 port);
    void updateSeason();
    void initParticules();
    void initTrees();
    void getNormal(Point O, Point X, Point Y);
    void drawModel(FileManager *model, float scale, Point position, QColor color);
    Camera* c;

    QTcpSocket *client;

private:
    bool master = false;
    int m_frame;
    QImage m_image;
    Point *p;


    int carte=1;


    int maj = 20;

    QTimer *timer;
    QTimer *timerParticule;
    QString *allSeasons;

    int currentSeason;

    Particule **particules;

    bool light;

    int nbTree;

    Point *posTrees;

    FileManager *winterTree;
    FileManager *summerTree;
    FileManager *springTree;
    FileManager *autumnTree;

public slots:
    void connected();
    void disconnected();
    void readyRead();

};

#endif // GAMEWINDOW_H
