#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "openglwindow.h"
#include "gamecamera.h"
#include "plyobject.h"

#include <QTimer>
#include <QOpenGLShaderProgram>

#define HIVER 0
#define PRINTEMPS 1
#define ETE 2
#define AUTOMNE 3

#define RIEN 0
#define NORMALE 1
#define PLUIE 2
#define NEIGE 3

struct point
{
    float x, y ,z;
};

class GameWindow : public OpenGLWindow
{
    Q_OBJECT
public:
    GameWindow(int fps, gamecamera* camera, int saison = PRINTEMPS);

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
    void setCamera(gamecamera* camera);
    gamecamera* getCamera();

    void initAll();
    void initEntity(int index);
    void update();

    int getCarte() const;
    void setCarte(int value);

    int getDay() const;
    void setDay(int day);

    void loadCustomMap();
    void displayPlyObject(PlyObject object, float x, float y, float z, float taille, float angle, bool displayFace);
    void load3dObjects();

    GLuint loadShader(GLenum type, const char *source);

    void testDiffus();
    void testSEV();
    void testNormalMap();
    void testDeform();

    void displayPoint(float x, float y, float z, float texX, float texY);
    void setTexture(QOpenGLShaderProgram * prog, float z);
public slots:
    void updateSeason();

protected:

    int m_frame;
    QImage m_image;
    point *p;

    int carte= 2;
    gamecamera* m_camera;
    bool m_hasToRotate = false;
    QTimer* m_timer;
    QTimer* m_calendar;
    int m_fps;
    int m_temps = RIEN;
    int m_saison = PRINTEMPS;
    int m_day;

    int         m_nb3dObject;
    PlyObject*  m_tab3dObjects;
    int*        m_tabArbresParType;
    int         m_sizeOfTab3d;
    float*      m_tabPos;
    int*        m_tabColor;
    float*      m_tabTaille;
    float*      m_tabRotation;

    QOpenGLShaderProgram *m_prog_diffus;
    QOpenGLShaderProgram *m_prog_sev;
    QOpenGLShaderProgram *m_prog_nm;
    QOpenGLShaderProgram *m_prog_deform;
    GLuint m_matrixUniform;
};


#endif // GAMEWINDOW_H
