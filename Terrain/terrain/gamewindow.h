#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "openglwindow.h"
#include "commonstruct.h"

#include <QTcpSocket>
#include <QAbstractSocket>
#include <QOpenGLShaderProgram>
#include <QScreen>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

class VertexData;

struct particles
{
    float x, y ,z;
    float falling_speed;
    float min_y;
};

class GameWindow : public OpenGLWindow
{
signals:

public slots:
    void connected();
    void disconnected();
    void readyRead();

public:
    GameWindow(int refresh_rate);
    ~GameWindow();

    void initialize();
    void render();
    bool event(QEvent *event);

    GLuint loadShader(GLenum type, const char *source);

    void restartTimer();

    void updateTitle();
    color seasonColor();

    void createParticles();
    particles newParticle();
    void displayParticles();

    void doConnect();

    void updateTerrainColor();

private:
    void updateEnumSaison();

    void openImage(QString str);
    void createTerrain();
    void displayTerrain();

    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent( QMouseEvent * event );
    void mouseReleaseEvent( QMouseEvent * event );
    void mouseMoveEvent(QMouseEvent* event);

public:
    static const unsigned int MAX_PARTICLES = 10000;
    static const unsigned int NB_ARBRES = 3;

private:

    int m_frame;

    int carte;
    int m_refresh_rate; 		// Taux de rafraîchissement de la fenêtre.

    bool share_cam;				// Permet de savoir si la caméra est partagée par plusieurs instances.

    QTimer* m_timer;			// Timer permettant un rafraîchissement à temps fixe de la fenêtre.

    float speed;				// Vitesse de rotation du terrain selon l'axe y quand animate = true.

    QString season;             // Contient le nom de la saison courante dans la fenêtre
    Saison saison;
    QTcpSocket *socket;         // QTcpSocket permettant de se connecter au serveur

    particles* tab_particles;   // Tableau contenant les particules de pluie/neige

    QOpenGLTexture* texture;
    QImage image;
    GLuint m_texture_location;

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_texAttr;
    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;

    QImage heightmap;                           //Image de heightmap

    int* hauteur;                               //Tableau des hauteurs, correspond aux pixels de l'image
    int terrain_height, terrain_width;          //Largeur et longueur du terrain

    VertexData* vertices;                       //Tableau de vertices
    GLushort* indices;                          //Tableau d'indices
    QOpenGLBuffer arrayBuf;                     //Buffer du tableau de vertices
    QOpenGLBuffer indexBuf;                     //Buffer du tableau d'indices
    bool wireframe;                             //Affichage fil de fer ou pas

    float direction_vue_h, direction_vue_v;     //angle vue caméra
    bool souris_active;                         //clique gauche enfoncé ou pas
};


#endif // GAMEWINDOW_H
