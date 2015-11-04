#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QCursor>
#include "QTimer"
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

#include "camera.h"
#include "openglwindow.h"
#include "clientthread.h"
#include "serverthread.h"
#include "snowparticles.h"
#include "rainparticles.h"
#include "drought.h"
#include "spring.h"
#include "utils.h"
#include "resourcemanager.h"
#include "plyentity.h"
#include "forest.h"

class GameWindow : public OpenGLWindow
{
    Q_OBJECT
public:
    GameWindow();
    GameWindow(Camera* camera, float framerate);

    void initialize();
    void render();
    void render(float delta);
    bool event(QEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void drawTriangles();
    void drawNormals();

    void displayTriangles();
    void displayLines();
    void displayTrianglesC();
    void displayPoints();
    void displayTrianglesTexture();

    void displayColor(float);

    void loadMap(QString localPath);

    QString serialize();
    void load();

public slots:
    void onSeasonChange();
    void onSaveRequest();
    void onLoadRequest();

signals:
    void requestSave();
    void requestLoad();

private:
    GLfloat *initVertices(GLint countX, GLint county);
    GLfloat getRandomZ(float x, float y);
    GLuint loadShader(GLenum type, const char *source);

    QString serializeVertices() const;

    int m_frame;
    QImage m_image;
    point *p;
    Camera *camera;
    ClientThread *cthread;
    ServerThread *sthread;
    QCursor* cursor;
    QCursor* cursor2;
    bool cursorCaptured = false;

    QVector<QVector3D> verticesArray;
    QVector<QVector3D> normalsArray;
    QVector<QVector3D> colorsArray;

    QOpenGLBuffer m_vertexbuffer;
    QOpenGLBuffer m_normalbuffer;
    QOpenGLBuffer m_colorbuffer;

    QOpenGLTexture *grassTexture;
    QOpenGLTexture *rockTexture;
    QOpenGLTexture *snowTexture;

    QOpenGLShaderProgram *shader;

    GLfloat* vertices;
    std::vector<point*> normals;
    QString imagePath;

    SnowParticles *snow;
    RainParticles *rain;
    Drought *drought;
    Spring *spring;

    int windowId;
    PlyEntity *entity;
    Forest *forest;

    bool displayNormals;

    int carte=1;

    int season = 0;
    int etat = 0;
    float rotX = -45.0;
    float rotY = -45.0;
    float ss = 1.0f;
    float framerate = 1.0f / 60.0f;
    float deltaTime = 0;
    float elapsed = 0;
    float lastUpdate = 0;
    bool fill = false;
    QTimer timer;

    GLuint textureId;
};



#endif // GAMEWINDOW_H
