#ifndef TRIANGLEWINDOW_H
#define TRIANGLEWINDOW_H

#include "openglwindow.h"
#include "filemanager.h"
#include "plyloader.h"
#include <QString>

/*struct point
{
    float x, y ,z;
};*/

class FileManager;



class paramCamera
{
public:
    float rotX = -45.0;
    float rotY = -45.0;
    float ss = 1.0f;
    float anim = 0.0f;

    int etat = 0;

    QString toSave();
    void loadParam(float anim, int etat, float rotX, float rotY, float ss);




};

class TriangleWindow : public OpenGLWindow
{
Q_OBJECT

public:
    TriangleWindow();
    TriangleWindow(int maj);
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
    void updateParticlesAut();
    void updateParticlesHiv();
    paramCamera* c;

    void setSeason(int );


    QString toSave();
    void loadWindow(float anim, int etat, float rotX,float rotY,float ss, int season, int day);
    QVector<PlyLoader*> getMods();
    void addMods(PlyLoader* ply);


public slots:
    void updateSeason();

private:
    int nbTick = 0;
    int m_frame = 0;
    int season, day;

    bool master = false;

    point* particules;
    QImage m_image;
    point *p;
    int carte=1;
    int maj = 20;

    QTimer *timer;
    QTimer *timerFPS;
    QVector<PlyLoader* > mods;
};



#endif // TRIANGLEWINDOW_H
