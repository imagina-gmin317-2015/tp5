#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "openglwindow.h"
#include "Camera.h"
#include "objetply3d.h"

#include <QList>

#define ETE 0
#define AUTOMNE 1
#define HIVER 2
#define PRINTEMPS 3

#define ARBRE_ETE 10
#define ARBRE_AUTOMNE 11
#define ARBRE_HIVER 12
#define ARBRE_PRINTEMPS 13
#define TONNEAU 14


struct point
{
    float x, y ,z;
};

#define MAX_PARTICLES 10000

typedef struct
{
    float x;
    float y;
    float z;
} tParticle1;

class GameWindow : public OpenGLWindow
{
    Q_OBJECT

public slots:
    void update_day();

public:
    GameWindow(int fps, Camera* camera, int day = 0);

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
    void displayColorSeasons(float);

    void loadMap(QString localPath);

    void initall();
    void initentity(int index);
    void renderMeteo();
    void update();

    int day() const;
    void setDay(int day);

    int type() const;
    void setType(int type);

    Camera *camera() const;
    void setCamera(Camera *camera);

    int getCarte() const;
    void setCarte(int value);

    bool getIsRotating() const;
    void setIsRotating(bool isRotating);

    QList<float> getObjectList() const;
    void setObjectList(const QList<float> &objectList);

    void renderObjectsFace();
    void renderObjectsPoints();
protected:

    ObjetPly3D m_arbreEte;
    ObjetPly3D m_arbreHiver;
    ObjetPly3D m_arbrePrintemps;
    ObjetPly3D m_arbreAutomne;
    ObjetPly3D m_tonneau;


    int m_day;
    int m_type;
    int m_frame;
    int carte=2;
    int m_fps;

    bool m_isRotating;

    //contient pour chaque objet les attributs suivant [type de l'objet, posX, posY, posZ, scale, rotX, rotY, rotZ]
    QList<float> m_objectList;

    QTimer * m_timer_day;
    QTimer * m_timer;

    Camera* m_camera;

    QImage m_image;

    point *p;
    tParticle1 Particles[MAX_PARTICLES];

};


#endif // GAMEWINDOW_H
