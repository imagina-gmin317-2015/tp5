#include "gamewindow.h"
#include "filemanager.h"

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
#include <QtOpenGL/QGLWidget>
#include <GL/glu.h>

using namespace std;

#define MAX_PARTICLES 10000

typedef struct
{
    float x;
    float y;
    float z;
} particle;

typedef struct
{
    float x;
    float y;
    float z;
}Point;

particle Particles[MAX_PARTICLES];
GLuint* textures;

int Nb_Paralleles = 16;
int Nb_Meridiens = 16;

void loadTexture(char *filename, GLuint &textureID);
void renderPyramide();
void renderGround();
void sphere();
void sphere (double rayon, float x, float y, float z);

GameWindow::GameWindow(int fps, gamecamera* camera, int saison)
    : m_saison(saison)
{
    this->m_camera = camera;
    m_timer = new QTimer(this);
    m_calendar = new QTimer(this);
    m_day = 0;

    connect(m_timer, SIGNAL(timeout()), this, SLOT(renderNow()));
    connect(m_calendar, SIGNAL(timeout()),this, SLOT(updateSeason()));

    m_timer->start(1000/fps);
    m_calendar->start(20);

    m_nb3dObject = 0;
//    m_sizeOfTab3d = 4;//Changer
//    m_tab3dObjects = new PlyObject[m_sizeOfTab3d];

//    m_tabArbresParType = new int[m_sizeOfTab3d];

//    m_tabPos = new float[3*m_nbArbres];
//    m_tabColor = new int[3*m_nbArbres];
//    m_tabTaille = new float[m_nbArbres];

    if (m_saison == PRINTEMPS)
    {
        m_day = 351;
    }
    else if (m_saison == ETE)
    {
        m_day = 81;
    }
    else if (m_saison == AUTOMNE)
    {
        m_day = 171;
    }
    else if (m_saison == HIVER)
    {
        m_day = 261;
    }

/*    m_tabArbresParType[0] = 2;
    m_tabArbresParType[1] = 1;
    m_tabArbresParType[2] = 1;
    m_tabArbresParType[3] = 1;

    m_tabPos[0] = 0;
    m_tabPos[1] = 0;
    m_tabPos[2] = 0;
    m_tabTaille[0] = 0.05;
    m_tabColor[0] = 0;
    m_tabColor[1] = 1;
    m_tabColor[2] = 0;

    m_tabPos[3] = 0.2;
    m_tabPos[4] = 0.2;
    m_tabPos[5] = 0;
    m_tabTaille[1] = 0.07;
    m_tabColor[3] = 0;
    m_tabColor[4] = 1;
    m_tabColor[5] = 0;*/

    this->m_fps = fps;
}

void GameWindow::initialize()
{
    const qreal retinaScale = devicePixelRatio();

    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-1.0, 1.0, -1.0, 1.0, -100.0, 100.0);

    glEnable(GL_LIGHTING);	// Active l'éclairage
    glEnable(GL_LIGHT0);	// Allume la lumière n°1

    glEnable( GL_COLOR_MATERIAL );

    float ambient[] = {1 , 1 , 1 , 1.0};
    float diffuse[] = {1 , 1 , 1 , 1.0};
    float position[] = {5 , 5 ,5 , 1.0};

    glLightfv(GL_LIGHT0 , GL_AMBIENT , ambient);
    glLightfv(GL_LIGHT0 , GL_DIFFUSE , diffuse);
    glLightfv(GL_LIGHT0 , GL_POSITION , position);

    QString depth ("..\\tp5\\images\\heightmap-");
    depth += QString::number(carte) ;
    depth += ".png" ;
    loadMap(depth);

    QString normalMapPath ("..\\tp5\\images\\normalmap-");
    normalMapPath += QString::number(carte) ;
    normalMapPath += ".png" ;
    QByteArray byte = normalMapPath.toLatin1();
    const char *normalMap2= byte.data();
    char* normalMap = const_cast<char *>(normalMap2);

    initAll();
    glEnable(GL_DEPTH_TEST);

    this->m_camera->setRotX(-20);
    this->m_camera->setRotY(0);
    textures = new GLuint[8];
    /*******************Partie 1**************** /
    loadTexture("..\\tp5\\images\\pyramide.jpg", textures[0]);
    loadTexture("..\\tp5\\images\\pyramide3.jpg", textures[1]);
    loadTexture("..\\tp5\\images\\tiles_ctf05r.jpg", textures[2]);
    loadTexture("..\\tp5\\images\\pyramide2.jpg", textures[3]);
    loadTexture("..\\tp5\\images\\sand.jpg", textures[4]);
    loadTexture("..\\tp5\\images\\EarthLow.jpg", textures[5]);
    loadTexture("..\\tp5\\images\\spheremaps.jpg", textures[6]);
    loadTexture("..\\tp5\\images\\earth_normal.jpg", textures[7]);
    /******************Partie 2*******************/
    loadTexture("..\\tp5\\images\\grass.png", textures[0]);
    loadTexture("..\\tp5\\images\\water2.jpg", textures[1]);
    loadTexture("..\\tp5\\images\\mountain.jpg", textures[2]);
    loadTexture("..\\tp5\\images\\snow.jpg", textures[3]);
    loadTexture("..\\tp5\\images\\heightmap-2.png", textures[4]);
    loadTexture("..\\tp5\\images\\autumn.jpg", textures[5]);
    loadTexture("..\\tp5\\images\\ice.jpg", textures[6]);
    loadTexture("..\\tp5\\images\\grass2.jpg", textures[7]);
    /******************************************/
    m_prog_diffus = new QOpenGLShaderProgram(this);
    m_prog_sev = new QOpenGLShaderProgram(this);
    m_prog_nm = new QOpenGLShaderProgram(this);
    m_prog_deform = new QOpenGLShaderProgram(this);

    m_prog_diffus->addShaderFromSourceFile(QOpenGLShader::Vertex, "..\\tp5\\shaders\\diffus_vertex_shader.vsh");
    m_prog_diffus->addShaderFromSourceFile(QOpenGLShader::Fragment, "..\\tp5\\shaders\\diffus_fragment_shader.fsh");
    m_prog_sev->addShaderFromSourceFile(QOpenGLShader::Vertex, "..\\tp5\\shaders\\SEV_vertex_shader.vsh");
    m_prog_sev->addShaderFromSourceFile(QOpenGLShader::Fragment, "..\\tp5\\shaders\\SEV_fragment_shader.fsh");
    m_prog_nm->addShaderFromSourceFile(QOpenGLShader::Vertex, "..\\tp5\\shaders\\NM_vertex_shader.vsh");
    m_prog_nm->addShaderFromSourceFile(QOpenGLShader::Fragment, "..\\tp5\\shaders\\NM_fragment_shader.fsh");
    m_prog_deform->addShaderFromSourceFile(QOpenGLShader::Vertex, "..\\tp5\\shaders\\deform_vertex_shader.vsh");
    m_prog_deform->addShaderFromSourceFile(QOpenGLShader::Fragment, "..\\tp5\\shaders\\deform_fragment_shader.fsh");

    if (!m_prog_diffus->link() && !m_prog_sev->link() && !m_prog_nm->link() && !m_prog_deform->link())
    {
        qDebug() << "ERRRROOOOOOOOOOOOOOOOOOORRR" << endl;
        qDebug() << m_prog_diffus->log();
        qDebug() << m_prog_sev->log();
        qDebug() << m_prog_nm->log();
        qDebug() << m_prog_deform->log();
        exit(0);
    }
}

void GameWindow::loadMap(QString localPath)
{
    if (QFile::exists(localPath))
    {
        m_image = QImage(localPath);
    }

    uint id = 0;
    p = new point[m_image.width() * m_image.height()];
    QRgb pixel;
    for(int i = 0; i < m_image.width(); i++)
    {
        for(int j = 0; j < m_image.height(); j++)
        {

            pixel = m_image.pixel(i,j);

            id = i*m_image.width() +j;

            p[id].x = (float)i/(m_image.width()) - ((float)m_image.width()/2.0)/m_image.width();
            p[id].y = (float)j/(m_image.height()) - ((float)m_image.height()/2.0)/m_image.height();
            p[id].z = 0.001f *(float)(qRed(pixel));
        }
    }
}

gamecamera* GameWindow::getCamera()
{
    return this->m_camera;
}

void GameWindow::setCamera(gamecamera* camera)
{
    this->m_camera = camera;
}

void GameWindow::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    update();
    glLoadIdentity();

    m_camera->scale();
    glRotatef(this->m_camera->getRotX(),1.0f,0.0f,0.0f);
    //gluPerspective(60.0f, 16.0f/9.0f, 0.1f, 100.0f);

    if(this->m_hasToRotate)
        this->m_camera->setRotY(this->m_camera->getRotY() + 1.0f);

    glRotatef(this->m_camera->getRotY(),0.0f,0.0f,1.0f);
    QMatrix4x4 matrix = QMatrix4x4();
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, m_camera->getScale());
    matrix.rotate(this->m_camera->getRotX(),1.0f,0.0f,0.0f);
    matrix.rotate(this->m_camera->getRotY(),0.0f,0.0f,1.0f);

    /**/
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[4]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, textures[3]);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, textures[5]);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, textures[6]);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, textures[7]);

    m_prog_nm->bind();
    m_matrixUniform = m_prog_nm->uniformLocation("matrix");
    m_prog_nm->setUniformValue(m_matrixUniform, matrix);
    glUniform1i(m_prog_nm->uniformLocation("texture2"), 0);
    glUniform1i(m_prog_nm->uniformLocation("texture1"), 1);
    /**/

    switch(this->m_camera->getEtat())
    {
    case 0:
    {
        displayPoints();
        //renderPyramide();
        //renderGround();

        //testDiffus();
        //testSEV();
        //testNormalMap();
        //testDeform();
        break;
    }
    case 1:
        displayLines();
        break;
    case 2:
        displayTriangles();
        break;
    case 3:
        displayTrianglesC();
        break;
    case 4:
        displayTrianglesTexture();
        break;
    case 5:
        //displayLines();
        displayTrianglesTexture();
        break;
    default:
        displayPoints();
        break;
    }

    /**/
    m_prog_nm->release();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_TEXTURE_2D);
    /**/

    if(m_nb3dObject > 0)
    {
        glColor3f(0, 1, 0);
        int k = 0;
        for(int i = 0; i < m_sizeOfTab3d; ++i)
        {
            for(int j = 0; j < m_tabArbresParType[i]; ++j)
            {
                glColor3f(m_tabColor[k]/255.0f, m_tabColor[k+1]/255.0f, m_tabColor[k+2]/255.0f);
                displayPlyObject(m_tab3dObjects[i], m_tabPos[k], m_tabPos[k+1], m_tabPos[k+2], m_tabTaille[k/3], m_tabRotation[k/3], true);
                k+=3;
            }
        }
    }

    /*glColor3f(128.0f/255.0f, 128.0f/255.0f, 128.0f/255.0f);
    displayPlyObject(tower, 0, 0.05, 0.05, 0.1, true);
    glColor3f(90.0f/255.0f, 90.0f/255.0f, 90.0f/255.0f);
    displayPlyObject(tower, 0, 0.05, 0.05, 0.1, false);
    glColor3f(0,0,139.0f/255);
    displayPlyObject(tardis, -0.07, 0.06, 0.35, 0.01, true);*/

    ++m_frame;
//    if(m_camera->m_temps != RIEN)
//    {
//        if(m_camera->m_temps == PLUIE || (m_camera->m_temps == NORMALE && m_saison == AUTOMNE))
//        {
//            glColor3f(0, 0, 1);
//            glBegin(GL_POINTS);

//            for(int i = 0; i <= MAX_PARTICLES; i++)
//            {
//                glVertex3f(Particles[i].x, Particles[i].y, Particles[i].z);
//            }
//            glEnd();
//        }
//        else if(m_camera->m_temps == NEIGE || (m_camera->m_temps == NORMALE && m_saison == HIVER))
//        {
//            glPointSize(1/2);
//            glColor3f(1, 1, 1);

//            glBegin(GL_POINTS);

//            for(int i = 0; i <= MAX_PARTICLES; i++)
//            {
//                glVertex3f(Particles[i].x, Particles[i].y, Particles[i].z);
//            }
//            glEnd();
//        }
//    }
}

bool GameWindow::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::UpdateRequest:

        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case 'Z':
        this->m_camera->setScale(this->m_camera->getScale()+0.10f);
        break;
    case 'S':
        this->m_camera->setScale(this->m_camera->getScale()-0.10f);
        break;
    case 'A':
        this->m_camera->setRotX(this->m_camera->getRotX()+1.0f);
        break;
    case 'E':
        this->m_camera->setRotX(this->m_camera->getRotX()-1.0f);
        break;
    case 'Q':
        this->m_camera->setRotY(this->m_camera->getRotY()+1.0f);
        break;
    case 'D':
        this->m_camera->setRotY(this->m_camera->getRotY()-1.0f);
        break;
    case 'W':
        this->m_camera->setEtat((this->m_camera->getEtat() + 1) % 6);
        break;
    case 'C':
        this->m_hasToRotate = !this->m_hasToRotate;
        break;
    case 'V':
        this->m_saison = (m_saison+ 1)%4;
        break;
    case 'F':
        this->m_camera->m_temps = (m_camera->m_temps+1) %4;
        break;
    case 'P':
        if(this->m_fps < 2000)
            this->m_fps *= 2;

        this->m_timer->stop();
        this->m_timer->start(1000/m_fps);
        break;
    case 'M':
        if(this->m_fps > 2)
            this->m_fps /= 2;

        this->m_timer->stop();
        this->m_timer->start(1000/m_fps);
        break;
    case 'O':
    {
        FileManager* fileManager = new FileManager(this);
        fileManager->saveData();
        fileManager->save3dObjects(m_tab3dObjects, m_tabArbresParType, m_sizeOfTab3d, m_tabPos,
                                   m_tabTaille, m_nb3dObject, m_tabColor, m_tabRotation);

        break;
    }
    case 'L':
    {
        loadCustomMap();
        load3dObjects();
        /*FileManager* fileManager = new FileManager(this);
        fileManager->read3dObjects();
        m_nb3dObject = fileManager->getNb3dObject();
        m_sizeOfTab3d = fileManager->getSizeOfTab3d();

        m_tab3dObjects = fileManager->getTab3dObjects();
        m_tabArbresParType = fileManager->getTabArbresParType();
        m_tabPos = fileManager->getTabPos();
        m_tabColor = fileManager->getTabColor();
        m_tabTaille = fileManager->getTabTaille();*/

        break;
    }
    case 'X':
        carte++;
        if(carte > 3)
            carte = 1;
        QString depth (":/heightmap-");
        depth += QString::number(carte) ;
        depth += ".png" ;

        loadMap(depth);
        break;
    }
    //renderNow();
}

void setTexturePrintemps(QOpenGLShaderProgram* prog, float z)
{
    if(z < 20*0.001)
        prog->setUniformValue(prog->uniformLocation("texture1"), 1);
    else if(z < 50*0.001)
        prog->setUniformValue(prog->uniformLocation("texture1"), 2);
    else if(z < 180*0.001)
        prog->setUniformValue(prog->uniformLocation("texture1"), 3);
    else
        prog->setUniformValue(prog->uniformLocation("texture1"), 4);
}

void setTextureEte(QOpenGLShaderProgram* prog, float z)
{
    if(z < 15*0.001)
        prog->setUniformValue(prog->uniformLocation("texture1"), 1);
    else if(z < 30*0.001)
        prog->setUniformValue(prog->uniformLocation("texture1"), 7);
    else if(z < 60*0.001)
        prog->setUniformValue(prog->uniformLocation("texture1"), 2);
    else//(z < 180*0.001)
        prog->setUniformValue(prog->uniformLocation("texture1"), 3);
}

void setTextureAutomne(QOpenGLShaderProgram* prog, float z)
{
    if(z < 15*0.001)
        prog->setUniformValue(prog->uniformLocation("texture1"), 1);
    else if(z < 40*0.001)
        prog->setUniformValue(prog->uniformLocation("texture1"), 2);
    else if(z < 60*0.001)
        prog->setUniformValue(prog->uniformLocation("texture1"), 5);
    else if(z < 150*0.001)
        prog->setUniformValue(prog->uniformLocation("texture1"), 3);
    else
        prog->setUniformValue(prog->uniformLocation("texture1"), 4);
}

void setTextureHiver(QOpenGLShaderProgram* prog, float z)
{
    if(z < 25*0.001)
        prog->setUniformValue(prog->uniformLocation("texture1"), 6);
    else if(z < 50*0.001)
        prog->setUniformValue(prog->uniformLocation("texture1"), 2);
    else if(z < 80*0.001)
        prog->setUniformValue(prog->uniformLocation("texture1"), 3);
    else
        prog->setUniformValue(prog->uniformLocation("texture1"), 4);
}

void GameWindow::setTexture(QOpenGLShaderProgram * prog, float z)
{
    if(m_saison == PRINTEMPS)
        setTexturePrintemps(prog, z);
    else if(m_saison == ETE)
        setTextureEte(prog, z);
    else if(m_saison == AUTOMNE)
        setTextureAutomne(prog, z);
    else if(m_saison == HIVER)
        setTextureHiver(prog, z);
}

void GameWindow::displayPoints()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    uint id = 0;

    for(int i = 0; i < m_image.width(); i++)
    {
        for(int j = 0; j < m_image.height(); j++)
        {

            id = i*m_image.width() +j;
            setTexture(m_prog_nm, p[id].z);

            glBegin(GL_POINTS);
            glTexCoord2f((float)i/(float)m_image.width(), (float)j/(float)m_image.height());
            //displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            glEnd();
        }
    }
}

void GameWindow::displayTriangles()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    uint id = 0;

    for(int i = 0; i < m_image.width()-1; i++)
    {
        for(int j = 0; j < m_image.height()-1; j++)
        {

            id = i*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);



            id = i*m_image.width() +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j+1;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
        }
    }

    glEnd();
}

void GameWindow::displayTrianglesC()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    uint id = 0;

    for(int i = 0; i < m_image.width()-1; i++)
    {
        for(int j = 0; j < m_image.height()-1; j++)
        {
            glColor3f(0.0f, 1.0f, 0.0f);
            id = i*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);


            glColor3f(1.0f, 1.0f, 1.0f);
            id = i*m_image.width() +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j+1;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
        }
    }
    glEnd();
}

void GameWindow::displayLines()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    uint id = 0;

    for(int i = 0; i < m_image.width()-1; i++)
    {
        for(int j = 0; j < m_image.height()-1; j++)
        {
            id = i*m_image.width() +j;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +(j+1);
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = (i+1)*m_image.width() +j;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +j;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = (i+1)*m_image.width() +j;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +(j+1);
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = i*m_image.width() +(j+1);
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j+1;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = (i+1)*m_image.width() +j+1;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = (i+1)*m_image.width() +(j);
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
        }
    }

    glEnd();
}

void GameWindow::displayTrianglesTexture()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    uint id = 0;

    for(int i = 0; i < m_image.width()-1; i++)
    {
        for(int j = 0; j < m_image.height()-1; j++)
        {
            id = i*m_image.width() +j;
            setTexture(m_prog_nm, p[id].z);

            glBegin(GL_TRIANGLES);
                displayPoint(p[id].x, p[id].y, p[id].z, (float)i/(float)m_image.width(), (float)j/(float)m_image.height());

                id = i*m_image.width() +(j+1);
                displayPoint(p[id].x, p[id].y, p[id].z, (float)i/(float)m_image.width(), (float)(j+1)/(float)m_image.height());

                id = (i+1)*m_image.width() +j;
                displayPoint(p[id].x, p[id].y, p[id].z, (float)(i+1)/(float)m_image.width(), (float)j/(float)m_image.height());
            glEnd();

            id = i*m_image.width() +(j+1);
            setTexture(m_prog_nm, p[id].z);

            glBegin(GL_TRIANGLES);
                displayPoint(p[id].x, p[id].y, p[id].z, (float)i/(float)m_image.width(), (float)(j+1)/(float)m_image.height());

                id = (i+1)*m_image.width() +j+1;
                displayPoint(p[id].x, p[id].y, p[id].z, (float)(i+1)/(float)m_image.width(), (float)(j+1)/(float)m_image.height());

                id = (i+1)*m_image.width() +j;
                displayPoint(p[id].x, p[id].y, p[id].z, (float)(i+1)/(float)m_image.width(), (float)j/(float)m_image.height());
            glEnd();
        }
    }
}

void getPrintempsColor(float z)
{
    if(z < 10.0f * 0.001f)
        glColor3f(65/255.0f, 105/255.0f, 225/255.0f);//Bleu foncé
    else if(z < 50.0f* 0.001f)
        glColor3f(46/255.0f, 160/255.0f, 87/255.0f);//Vert
    else if(z < 200.0f* 0.001f)
        glColor3f(126/255.0f, 88/255.0f, 53/255.0f);//Marron
    else
        glColor3f(220/255.0f, 220/255.0f, 220/255.0f);//Gris clair
}

void getEteColor(float z)
{
    if(z < 10.0f * 0.001f)
        glColor3f(65/255.0f, 105/255.0f, 225/255.0f);//Bleu foncé
    else if(z < 50.0f* 0.001f)
        glColor3f(20/255.0f, 139/255.0f, 40/255.0f);//Vert
    else //if(z < 130.0f* 0.001f)
        glColor3f(126/255.0f, 88/255.0f, 53/255.0f);//Marron
}

void getAutomneColor(float z)
{
    if(z < 10.0f * 0.001f)
        glColor3f(65/255.0f, 105/255.0f, 225/255.0f);//Bleu foncé
    else if(z < 35.0f* 0.001f)
        glColor3f(46/255.0f, 90/255.0f, 87/255.0f);//Vert foncé
    else if(z < 80.0f* 0.001f)
        glColor3f(247/255.0f, 166/255.0f, 59/255.0f);//Orange
    else if(z < 130.0f* 0.001f)
        glColor3f(126/255.0f, 88/255.0f, 53/255.0f);//Marron
    else// if(z < 200.0f* 0.001f)
        glColor3f(220/255.0f, 220/255.0f, 220/255.0f);//Gris clair
}

void getHiverColor(float z)
{
    if(z < 10.0f * 0.001f)
        glColor3f(150/255.0f, 150/255.0f, 225/255.0f);//Bleu foncé
    else if(z < 40.0f* 0.001f)
         glColor3f(220/255.0f, 220/255.0f, 220/255.0f);//Gris clair
    else if(z < 60.0f* 0.001f)
        glColor3f(126/255.0f, 88/255.0f, 53/255.0f);//Marron
    else
        glColor3f(1, 1, 1);//Blanc
}

void GameWindow::displayColor(float alt)
{
    if(m_saison == PRINTEMPS)
        getPrintempsColor(alt);
    else if(m_saison == ETE)
        getEteColor(alt);
    else if(m_saison == AUTOMNE)
        getAutomneColor(alt);
    else if(m_saison == HIVER)
        getHiverColor(alt);
}

void GameWindow::initAll()
{
    for(int i = 0; i <= MAX_PARTICLES; i++)
    {
        Particles[i].x = -0.5 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.5+0.5)));
        Particles[i].y = -0.5+ static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.5+0.5)));
        Particles[i].z = 1 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(-1)));
    }
}

void GameWindow::initEntity(int i)
{
    Particles[i].x = -0.5 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.5+0.5)));
    Particles[i].y = -0.5+ static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.5+0.5)));
    Particles[i].z = 1 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(-1)));
}

void GameWindow::update()
{
    if(m_camera->m_temps == PLUIE || (m_camera->m_temps == NORMALE && m_saison == AUTOMNE) )
    {
        for(int i = 0; i <= MAX_PARTICLES; i++)
        {
            Particles[i].z -= (rand() % 20*0.001f);

            if(Particles[i].z <= 0.1)
            {
                initEntity(i);
            }
        }
    }
    else if(m_camera->m_temps == NEIGE || (m_camera->m_temps == NORMALE && m_saison == HIVER) )
    {
        for(int i = 0; i <= MAX_PARTICLES; i++)
        {
            Particles[i].x += -0.005+ static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.005+0.005)));
            Particles[i].z -= (rand() % 10*0.001f);

            if(Particles[i].z <= 0.1)
            {
                initEntity(i);
            }
        }
    }
}

void GameWindow::updateSeason()
{
    m_day = (m_day + 1) % 365;

    if (m_day <= 80)
        m_saison = PRINTEMPS;
    else if (m_day <= 170)
        m_saison = ETE;
    else if (m_day <= 260)
        m_saison = AUTOMNE;
    else if (m_day <=350)
        m_saison = HIVER;

    m_calendar->start(20);
}

int GameWindow::getDay() const
{
    return m_day;
}

void GameWindow::setDay(int day)
{
    m_day = day;
}

int GameWindow::getCarte() const
{
    return carte;
}

void GameWindow::setCarte(int value)
{
    carte = value;
}

void GameWindow::loadCustomMap()
{
    FileManager* fileManager = new FileManager(this);
    fileManager->readData();
    this->carte = fileManager->carte();
    QString depth (":/heightmap-");
    depth += QString::number(carte) ;
    depth += ".png" ;
    loadMap(depth);
    this->m_day = fileManager->day();
    this->m_camera->setEtat(fileManager->etat());
    this->m_camera->setRotX(fileManager->rotX());
    this->m_camera->setRotY(fileManager->rotY());
    this->m_camera->setScale(fileManager->ss());
}

void GameWindow::displayPlyObject(PlyObject object, float x, float y, float z, float taille, float angle, bool displayFace)
{
    float* tabPoints = object.getTabPoints();
    int* tabFaces = object.getTabFaces();
    int nbFaces = object.getNbFaces();
    glPushMatrix();
    glRotatef(angle, 0, 0, 1);
    if(displayFace)
    {
        for(int i = 0; i < nbFaces*4; i +=4)
        {
            if(tabFaces[i+3] != -1)
            {
                glBegin(GL_QUADS);
                glVertex3f(x + tabPoints[tabFaces[i]*3]*taille,   y + tabPoints[tabFaces[i]*3 + 1]*taille,   z + tabPoints[tabFaces[i]*3 + 2]*taille);
                glVertex3f(x + tabPoints[tabFaces[i+1]*3]*taille, y + tabPoints[tabFaces[i+1]*3 + 1]*taille, z + tabPoints[tabFaces[i+1]*3 + 2]*taille);
                glVertex3f(x + tabPoints[tabFaces[i+2]*3]*taille, y + tabPoints[tabFaces[i+2]*3 + 1]*taille, z + tabPoints[tabFaces[i+2]*3 + 2]*taille);
                glVertex3f(x + tabPoints[tabFaces[i+3]*3]*taille, y + tabPoints[tabFaces[i+3]*3 + 1]*taille, z + tabPoints[tabFaces[i+3]*3 + 2]*taille);
                glEnd();
            }
            else
            {
                glBegin(GL_TRIANGLES);
                glVertex3f(x + tabPoints[tabFaces[i]*3]*taille,   y + tabPoints[tabFaces[i]*3 + 1]*taille,   z + tabPoints[tabFaces[i]*3 + 2]*taille);
                glVertex3f(x + tabPoints[tabFaces[i+1]*3]*taille, y + tabPoints[tabFaces[i+1]*3 + 1]*taille, z + tabPoints[tabFaces[i+1]*3 + 2]*taille);
                glVertex3f(x + tabPoints[tabFaces[i+2]*3]*taille, y + tabPoints[tabFaces[i+2]*3 + 1]*taille, z + tabPoints[tabFaces[i+2]*3 + 2]*taille);
                glEnd();
            }
        }
    }
    else
    {
        glBegin(GL_LINES);
        for(int i = 0; i < nbFaces*4; i +=4)
        {
            glVertex3f(x + tabPoints[tabFaces[i]*3]*taille,   y + tabPoints[tabFaces[i]*3 + 1]*taille,   z + tabPoints[tabFaces[i]*3 + 2]*taille);
            glVertex3f(x + tabPoints[tabFaces[i+1]*3]*taille, y + tabPoints[tabFaces[i+1]*3 + 1]*taille, z + tabPoints[tabFaces[i+1]*3 + 2]*taille);
            glVertex3f(x + tabPoints[tabFaces[i+2]*3]*taille, y + tabPoints[tabFaces[i+2]*3 + 1]*taille, z + tabPoints[tabFaces[i+2]*3 + 2]*taille);
            glVertex3f(x + tabPoints[tabFaces[i+3]*3]*taille, y + tabPoints[tabFaces[i+3]*3 + 1]*taille, z + tabPoints[tabFaces[i+3]*3 + 2]*taille);
        }
        glEnd();
    }
    glPopMatrix();
}

void GameWindow::load3dObjects()
{
    FileManager* fileManager = new FileManager(this);
    fileManager->read3dObjects();
    m_nb3dObject = fileManager->getNb3dObject();
    m_sizeOfTab3d = fileManager->getSizeOfTab3d();

    m_tab3dObjects = fileManager->getTab3dObjects();
    m_tabArbresParType = fileManager->getTabArbresParType();
    m_tabPos = fileManager->getTabPos();
    m_tabColor = fileManager->getTabColor();
    m_tabTaille = fileManager->getTabTaille();
    m_tabRotation = fileManager->getTabRotation();
}

void loadTexture(char *filename, GLuint &textureID)
{
    glEnable(GL_TEXTURE_2D); // Enable texturing

    glGenTextures(1, &textureID); // Obtain an id for the texture
    glBindTexture(GL_TEXTURE_2D, textureID); // Set as the current texture

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    QImage im(filename);
    QImage tex = QGLWidget::convertToGLFormat(im);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    glDisable(GL_TEXTURE_2D);

    //return tex;
}

void renderPyramide()
{
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glBegin(GL_TRIANGLES);
        glTexCoord2f(0.5,1); glVertex3f(0, 1, 0);
        glTexCoord2f(0,0); glVertex3f(-0.5f, -0.5f, 0.5);
        glTexCoord2f(1,0); glVertex3f(0.5f, -0.5f, 0.5);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glBegin(GL_TRIANGLES);
        glTexCoord2f(0.5,1); glVertex3f(0, 1, 0);
        glTexCoord2f(0,0); glVertex3f(-0.5, -0.5, -0.5);
        glTexCoord2f(1,0); glVertex3f(0.5f, -0.5f, -0.5);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glBegin(GL_TRIANGLES);
        glTexCoord2f(0.5,1); glVertex3f(0, 1, 0);
        glTexCoord2f(0,0); glVertex3f(-0.5f, -0.5f, 0.5);
        glTexCoord2f(1,0); glVertex3f(-0.5f, -0.5f, -0.5);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, textures[3]);
    glBegin(GL_TRIANGLES);
        glTexCoord2f(0.5,1); glVertex3f(0, 1, 0);
        glTexCoord2f(0,0); glVertex3f(0.5f, -0.5f, 0.5);
        glTexCoord2f(1,0); glVertex3f(0.5f, -0.5f, -0.5);
    glEnd();


    glDisable(GL_TEXTURE_2D);
}

void renderGround()
{
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, textures[4]);
    glBegin(GL_QUAD_STRIP);
        glTexCoord2f(0,0); glVertex3f(1,-1,1);
        glTexCoord2f(0,5); glVertex3f(1,-1,-1);
        glTexCoord2f(5,0); glVertex3f(-1,-1,1);
        glTexCoord2f(5,5); glVertex3f(-1,-1,-1);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void sphere (double rayon, float x, float y, float z)
{
    Point** tabPoint = new Point* [Nb_Paralleles];
    double theta = 0, phi = 0;

    for (int i = 0; i < Nb_Paralleles; ++i)
    {
        tabPoint[i] = new Point [Nb_Meridiens+1];

        theta = 0;
        for(int j = 0; j < Nb_Meridiens; ++j)
        {
            Point pt;
            pt.x = rayon*sin(phi*M_PI/180.0)*cos(theta*M_PI/180.0);
            pt.y = rayon*sin(phi*M_PI/180.0)*sin(theta*M_PI/180.0);
            pt.z = rayon*cos(phi*M_PI/180.0);
            tabPoint[i][j] = pt;

            theta += (360.0/Nb_Meridiens);
        }

        Point pt;
        pt.x = rayon*sin(phi*M_PI/180.0)*cos(M_PI/180.0);
        pt.y = rayon*sin(phi*M_PI/180.0)*sin(M_PI/180.0);
        pt.z = rayon*cos(phi*M_PI/180.0);
        tabPoint[i][Nb_Meridiens] = pt;

        phi += (180.0/Nb_Paralleles);
    }

    //glBegin(GL_QUADS);
    glColor3f(0.7,0.7,0);
    QVector<QVector2D> m_textures;
    QVector<QVector3D>  m_vertexarray;
    QVector2D coordonnees;

    for(int i = 0; i < Nb_Paralleles; ++i)
    {
        for(int j = 0; j < Nb_Meridiens; ++j)
        {
            //glVertex3f
            m_vertexarray.push_back(QVector3D(tabPoint[i][j].x + x , tabPoint[i][j].y + y, tabPoint[i][j].z + z));
            m_vertexarray.push_back(QVector3D(tabPoint[(i+1)%Nb_Paralleles][j].x + x, tabPoint[(i+1)%Nb_Paralleles][j].y + y,
                       tabPoint[(i+1)%Nb_Paralleles][j].z + z));
            m_vertexarray.push_back(QVector3D(tabPoint[(i+1)%Nb_Paralleles][(j+1)%Nb_Meridiens].x + x,
                        tabPoint[(i+1)%Nb_Paralleles][(j+1)%Nb_Meridiens].y + y,
                        tabPoint[(i+1)%Nb_Paralleles][(j+1)%Nb_Meridiens].z + z));
            m_vertexarray.push_back(QVector3D(tabPoint[i][(j+1)%Nb_Meridiens].x + x, tabPoint[i][(j+1)%Nb_Meridiens].y + y,
                       tabPoint[i][(j+1)%Nb_Meridiens].z + z));

            coordonnees.setX(x + tabPoint[i][j].x /Nb_Paralleles);
            coordonnees.setY(y + tabPoint[i][j].y /Nb_Meridiens);
            m_textures.push_back(coordonnees);
        }
    }
    //glEnd();
    //qglColor(Qt::white);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, m_vertexarray.constData());
    glDrawArrays(GL_QUADS, 0, m_vertexarray.size());
    glDisableClientState(GL_VERTEX_ARRAY);

    //glBegin(GL_TRIANGLES);
m_vertexarray.clear();
    for(int j = 0; j < Nb_Meridiens; ++j)
    {
        m_vertexarray.push_back(QVector3D(x, y, -rayon+z));
        m_vertexarray.push_back(QVector3D(tabPoint[Nb_Paralleles-1][j].x + x, tabPoint[Nb_Paralleles-1][j].y + y, tabPoint[Nb_Paralleles-1][j].z + z));
        m_vertexarray.push_back(QVector3D(tabPoint[Nb_Paralleles-1][(j+1)%Nb_Meridiens].x + x,
                tabPoint[Nb_Paralleles-1][(j+1)%Nb_Meridiens].y + y,
                tabPoint[Nb_Paralleles-1][(j+1)%Nb_Meridiens].z + z));
    }
    //glEnd();

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, m_vertexarray.constData());
    glDrawArrays(GL_TRIANGLES, 0, m_vertexarray.size());
    glDisableClientState(GL_VERTEX_ARRAY);

    QOpenGLBuffer m_texturebuffer (QOpenGLBuffer::VertexBuffer);
    m_texturebuffer.create();
    m_texturebuffer.bind();
    m_texturebuffer.allocate(m_textures.constData(), sizeof(QVector2D) * m_textures.size());
    m_texturebuffer.release();
}

void sphere()
{
    //glPushMatrix(); //pour que les transformations soient r�versibles
    //glEnable(GL_TEXTURE_2D);

    //glBindTexture(GL_TEXTURE_2D,textures[6]);

    GLUquadric* params = gluNewQuadric();

    gluQuadricDrawStyle(params,GLU_FILL);
    gluQuadricTexture(params,GL_TRUE);
    //glTranslated(0,0,1);

    gluSphere(params,0.5,20,20);

    gluDeleteQuadric(params);
    //glDisable(GL_TEXTURE_2D);

    //glPopMatrix(); //hop je remets tout comme je l'ai trouv�
}

void DrawRocket()
{
    glPushMatrix(); //pour que les transformations soient réversibles

    GLUquadric* params = gluNewQuadric(); //création du quadrique
    gluQuadricTexture(params,GL_TRUE); //activation des coordonnées de texture

    //glBindTexture(GL_TEXTURE_2D,textures[0]); //texture du haut
    gluCylinder(params,0.5,0,1.6,20,1); //cône 1

    glBindTexture(GL_TEXTURE_2D,textures[1]);
    glTranslated(0,0,-1.05); //je descends pour faire le 2ème cône
    gluCylinder(params,0.15,0.5,1.05,20,1); //cône 2

    glBindTexture(GL_TEXTURE_2D,textures[2]);
    glTranslated(0,0,-0.25); //je descends enfin tout en bas (sur le schéma)
    gluCylinder(params,0.3,0.15,0.25,20,1); //cône 3

    //et à la même position je dessine le disque de sortie des flammes
    glBindTexture(GL_TEXTURE_2D,textures[3]);
    gluDisk(params,0,0.3,20,1); //disque 4

    gluDeleteQuadric(params); //je supprime le quadrique

    glPopMatrix(); //hop je remets tout comme je l'ai trouvé
}

void GameWindow::testDiffus()
{
    m_matrixUniform = m_prog_diffus->uniformLocation("matrix");
    m_prog_diffus->bind();
        QMatrix4x4 matrix;
        matrix.perspective(60.0f, 16.0f/9.0f, 0.1f, 100.0f);
        matrix.translate(0, 0, m_camera->getScale());
        matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);
        matrix.rotate(this->m_camera->getRotX(),1.0f,0.0f,0.0f);
        matrix.rotate(this->m_camera->getRotY(),0.0f,0.0f,1.0f);
        m_prog_diffus->setUniformValue(m_matrixUniform, matrix);
        //m_program->setUniformValue("light_position", QVector4D(1.0, 1.0, -10.0, 1.0));

        renderPyramide();

        //glBindTexture(GL_TEXTURE_2D, textures[5]);

        //sphere();

        glBindTexture(GL_TEXTURE_2D, textures[4]);
        matrix = QMatrix4x4();
        matrix.perspective(60.0f, 16.0f/9.0f, 0.1f, 100.0f);
        matrix.translate(0, 0, m_camera->getScale());
        matrix.rotate(this->m_camera->getRotX(),1.0f,0.0f,0.0f);
        matrix.rotate(this->m_camera->getRotY(),0.0f,0.0f,1.0f);
        m_prog_diffus->setUniformValue(m_matrixUniform, matrix);
        renderGround();
    m_prog_diffus->release();
}

void GameWindow::testSEV()
{
    m_prog_sev->bind();
        QMatrix4x4 matrix;
        matrix.perspective(60.0f, 16.0f/9.0f, 0.1f, 100.0f);
        matrix.translate(0, 0, m_camera->getScale());
        matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);
        matrix.rotate(this->m_camera->getRotX(),1.0f,0.0f,0.0f);
        matrix.rotate(this->m_camera->getRotY(),0.0f,0.0f,1.0f);
        m_prog_sev->setUniformValue(m_matrixUniform, matrix);
        //m_program->setUniformValue("light_position", QVector4D(1.0, 1.0, -10.0, 1.0));

        glUniform1i(m_prog_sev->uniformLocation("texture1"), 0);
        glUniform1i(m_prog_sev->uniformLocation("texture2"), 1);

        //renderPyramide();

        //reflected texture
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[6]);

        //texture 0, first texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[5]);

        sphere();

        //remove reflect texture
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[4]);
        matrix = QMatrix4x4();
        matrix.perspective(60.0f, 16.0f/9.0f, 0.1f, 100.0f);
        matrix.translate(0, 0, m_camera->getScale());
        matrix.rotate(this->m_camera->getRotX(),1.0f,0.0f,0.0f);
        matrix.rotate(this->m_camera->getRotY(),0.0f,0.0f,1.0f);
        m_prog_sev->setUniformValue(m_matrixUniform, matrix);
        renderGround();
    m_prog_sev->release();
}

void GameWindow::testNormalMap()
{
    m_matrixUniform = m_prog_nm->uniformLocation("matrix");

    m_prog_nm->bind();
        QMatrix4x4 matrix;
        matrix.perspective(60.0f, 16.0f/9.0f, 0.1f, 100.0f);
        matrix.translate(0, 0, m_camera->getScale());
        matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);
        matrix.rotate(this->m_camera->getRotX(),1.0f,0.0f,0.0f);
        matrix.rotate(this->m_camera->getRotY(),0.0f,0.0f,1.0f);
        m_prog_nm->setUniformValue(m_matrixUniform, matrix);
        //m_program->setUniformValue("light_position", QVector4D(1.0, 1.0, -10.0, 1.0));

        glUniform1i(m_prog_nm->uniformLocation("texture1"), 0);
        glUniform1i(m_prog_nm->uniformLocation("texture2"), 1);

        //renderPyramide();

        //reflected texture
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[7]);

        //texture 0, first texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[5]);

        sphere();

        //remove reflect texture
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[4]);
        matrix = QMatrix4x4();
        matrix.perspective(60.0f, 16.0f/9.0f, 0.1f, 100.0f);
        matrix.translate(0, 0, m_camera->getScale());
        matrix.rotate(this->m_camera->getRotX(),1.0f,0.0f,0.0f);
        matrix.rotate(this->m_camera->getRotY(),0.0f,0.0f,1.0f);
        m_prog_nm->setUniformValue(m_matrixUniform, matrix);
        renderGround();
    m_prog_nm->release();
}

void GameWindow::testDeform()
{
    m_matrixUniform = m_prog_deform->uniformLocation("matrix");
    m_prog_deform->bind();
        QMatrix4x4 matrix;
        matrix.perspective(60.0f, 16.0f/9.0f, 0.1f, 100.0f);
        matrix.translate(0, 0, m_camera->getScale());
        matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);
        matrix.rotate(this->m_camera->getRotX(),1.0f,0.0f,0.0f);
        matrix.rotate(this->m_camera->getRotY(),0.0f,0.0f,1.0f);
        m_prog_deform->setUniformValue(m_matrixUniform, matrix);
        //m_program->setUniformValue("light_position", QVector4D(1.0, 1.0, -10.0, 1.0));

        //renderPyramide();

        glBindTexture(GL_TEXTURE_2D, textures[5]);

        sphere();

        glBindTexture(GL_TEXTURE_2D, textures[4]);
        matrix = QMatrix4x4();
        matrix.perspective(60.0f, 16.0f/9.0f, 0.1f, 100.0f);
        matrix.translate(0, 0, m_camera->getScale());
        matrix.rotate(this->m_camera->getRotX(),1.0f,0.0f,0.0f);
        matrix.rotate(this->m_camera->getRotY(),0.0f,0.0f,1.0f);
        m_prog_deform->setUniformValue(m_matrixUniform, matrix);
        renderGround();
    m_prog_deform->release();
}

void GameWindow::displayPoint(float x, float y, float z, float texX, float texY)
{
    glTexCoord2f(texX*2, texY*2);
    glVertex3f( x, y, z);
}

