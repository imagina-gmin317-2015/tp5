#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "openglwindow.h"
#include "camera.h"
#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtOpenGL/QGLWidget>


struct point
{
    float x, y ,z;
};

struct face
{
    int a, b, c, d;
};


class GameWindow : public OpenGLWindow
{
    Q_OBJECT
public:
    GameWindow(int fps, Camera *camera, int season);
    GameWindow(int fps, Camera *camera);

    void initialize();
    void render();
    bool event(QEvent *event);
    void initall();
    void initentity(int index);
    void update();

    void keyPressEvent(QKeyEvent *event);

    void displayTriangles();
    void displayLines();
    void displayTrianglesC();
    void displayPoints();
    void displayTrianglesTexture();

    void displayColor(float);

    void loadMap(QString localPath);

    Camera * getCamera();
    void setCamera(Camera * camera);
    void setSeason(int numSeason);

    int getCarte() const;
    int getSeason() const;
    int getDay() const;

    void setDay(int day);

    void setCarte(int value);

    point *getAllSommets() const;
    void setAllSommets(point *allSommets);

    face *getAllFaces() const;
    void setAllFaces(face *allFaces);

    int getNbSommets() const;
    void setNbSommets(int nbSommets);

    int getNbFaces() const;
    void setNbFaces(int nbFaces);

    point *getPosObject() const;
    void setPosObject(point * posObject);


    float getTailleObject() const;
    void setTailleObject(float tailleObject);

    QString getNameObject() const;
    void setNameObject(const QString &nameObject);

    int getNbObject() const;
    void setNbObject(int nbObject);

    int getObjectLoaded() const;
    void setObjectLoaded(int objectLoaded);

    void loadObjectFile();

public slots:
    void incrDay();


private :
    void initDay();
    void saveMap();
    void loadCustomMap();
    void showObjectMap();
    QImage loadTexture(char *filename, GLuint &textureID);
    void displayTexture(uint id);
    void displayNormaleTexture(uint id);


protected:

    QOpenGLShaderProgram *m_diffus;
    QOpenGLShaderProgram *m_NM;

    GLuint m_matrixUniform;
    GLuint tabTexture[10];

    int m_frame;
    QImage m_image;
    point *p;

    Camera * m_camera;
    int carte=1;

    bool m_isRotating;

    QTimer * m_timer;
    int m_fps;

    // SEASON
    QTimer * m_timer_season;
    int m_season;
    float m_neigeAlt;
    float m_eauAlt;

    int m_day;

    // OBJECT PLY
    point * m_allSommets;

    face * m_allFaces;
    int m_nbSommets;
    int m_nbFaces;

    int m_nbObject;
    float m_tailleObject;
    point * m_posObject;
    int m_objectLoaded;

    QString m_nameObject;

};


#endif // GAMEWINDOW_H
