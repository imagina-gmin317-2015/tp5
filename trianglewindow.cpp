#include "trianglewindow.h"

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

//#include <omp.h>

int numParticules = 1000;
int minP = 0;
int maxP = 360;


using namespace std;

GLubyte Texture[16] =
{
    254,254,254,0xFF, 130,196,108,0xFF,
    148,127,96,0xFF, 49,140,231,0xFF
};

GLuint Name;

TriangleWindow::TriangleWindow()
{
    nbTick = 0;
    m_frame = 0;
    maj = 20;
    QString s ("FPS : ");
    s += QString::number(1000/maj);
    s += "(";
    s += QString::number(maj);
    s += ")";
    setTitle(s);
    timer = new QTimer();
    timer->connect(timer, SIGNAL(timeout()),this, SLOT(renderNow()));
    timer->start(maj);

    master = true;
}
TriangleWindow::TriangleWindow(int _maj)
{
    nbTick = 0;
    m_frame = 0;
    maj = _maj;
    QString s ("FPS : ");
    s += QString::number(1000/maj);
    s += "(";
    s += QString::number(maj);
    s += ")";
    setTitle(s);
    timer = new QTimer();
    timer->connect(timer, SIGNAL(timeout()),this, SLOT(renderNow()));
    timer->start(maj);
}

void TriangleWindow::setSeason(int i)
{
    season = i;

    if (i==0) day=80;
    else if (i==1) day = 170;
    else if (i==2) day = 260;
    else if (i==3) day = 350;
}
/*
 * Format de sauvegarde :  anim;etat;rotX;rotY;ss;nbSaison;nbDay
 */

QString TriangleWindow::toSave()
{
    QString st="";
    st+=this->c->toSave()+";"+QString::number(season)+";"+QString::number(day) ;
    return st;

}

void TriangleWindow::loadWindow(float anim, int etat, float rotX, float rotY, float ss, int season, int day)
{
    this->c->loadParam(anim,etat,rotX,rotY,ss);
    this->season = season;
    this->day = day;
}

QVector<PlyLoader *> TriangleWindow::getMods()
{
    return this->mods;
}

void TriangleWindow::addMods(PlyLoader *ply)
{
    this->mods.push_back(ply);
}


void TriangleWindow::updateSeason()
{
    day = (day + 1) % 365;

    if (day==80) season = 0;
    else if (day==170) season = 1;
    else if (day==260) season = 2;
    else if (day==350) season = 3;
}


void TriangleWindow::initialize()
{
    const qreal retinaScale = devicePixelRatio();


    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -100.0, 100.0);


    /* Lumière */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Create light components
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { -0.5f, 0.5f, -4.0f, 0.0f };

    // Assign created components to GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    // Textures
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1,&Name);
    glBindTexture(GL_TEXTURE_2D,Name);
    glTexImage2D(GL_TEXTURE_2D,0,4,2,2,0,GL_RGBA,GL_UNSIGNED_BYTE,Texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);



    loadMap(":/heightmap-1.png");

    particules = new point[numParticules];



    for(int i = 0; i < numParticules; i++)
    {
        int angle =minP + (rand() % (int)(maxP - minP + 1));
        int dist = (rand() % (int)(100 ));
        int alt = (rand() % (int)(100));
        float x = sin(
                    ((3.14159 * 2) *
                     angle
                     )/360
                    )*dist;
        float y = cos(
                    ((3.14159 * 2) *
                     angle
                     )/360
                    )*dist;

        particules[i].x = 0.5f;
        particules[i].y = 0.5f;
        particules[i].z = (float)(alt)/100;
    }

    PlyLoader *l = new PlyLoader(":/summertree.ply");
    l->load();
    addMods(l);

}

void TriangleWindow::loadMap(QString localPath)
{

    if (QFile::exists(localPath)) {
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
            p[id].z = 0.001f * (float)(qRed(pixel));

        }
    }
}

void TriangleWindow::render()
{
    nbTick += maj;

    if(nbTick >= 1000)
    {
        QString s ("FPS : ");
        s += QString::number(m_frame);
        s += "(th=";
        s += QString::number(1000/maj);
        s += "-";
        s += QString::number(maj);
        s += ")";
        s += " day ";
        s += QString::number(day);
        if (season==0) s += " Printemps ";
        else if (season==1) s += " Eté ";
        if (season==2) s += " Automne ";
        if (season==3) s += " Hiver ";
        setTitle(s);
        nbTick = 0;
        m_frame = 0;
    }
    glClear(GL_COLOR_BUFFER_BIT);
    glClearDepth(1);


    glLoadIdentity();
    glScalef(c->ss,c->ss,c->ss);

    glRotatef(c->rotX,1.0f,0.0f,0.0f);
    if(c->anim == 0.0f)
    {
        glRotatef(c->rotY,0.0f,0.0f,1.0f);
    }
    else
    {
        glRotatef(c->anim,0.0f,0.0f,1.0f);
        if(master)
            c->anim +=0.05f;
    }



    glNormal3f(0,0,-1);
    switch(c->etat)
    {
    case 0:
        displayPoints();
        break;
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

        displayTrianglesTexture();
        displayLines();
        break;
    default:
        displayPoints();
        break;
    }
    if (season == 2)
        updateParticlesAut();
    else if (season == 3)
        updateParticlesHiv();

    for(int i=0;i<mods.size();i++){
        mods.at(i)->draw();
    }

    m_frame++;

}

bool TriangleWindow::event(QEvent *event)
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

void TriangleWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {

    case Qt::Key_F6:
        FileManager::Instance()->save();
        break;
    case Qt::Key_F5:
        FileManager::Instance()->load();
        break;
    case 'C':
        if(c->anim == 0.0f)
            c->anim = c->rotY;
        else
            c->anim = 0.0f;
        break;
    case 'Z':
        c->ss += 0.10f;
        break;
    case 'S':
        c->ss -= 0.10f;
        break;
    case 'A':
        c->rotX += 1.0f;
        break;
    case 'E':
        c->rotX -= 1.0f;
        break;
    case 'Q':
        c->rotY += 1.0f;
        break;
    case 'D':
        c->rotY -= 1.0f;
        break;
    case 'W':
        c->etat ++;
        if(c->etat > 5)
            c->etat = 0;
        break;
    case 'P':
        maj++;
        timer->stop();
        timer->start(maj);
        break;
    case 'O':
        maj--;
        if(maj < 1)
            maj = 1;
        timer->stop();
        timer->start(maj);
        break;
    case 'L':
        maj = maj - 20;
        if(maj < 1)
            maj = 1;
        timer->stop();
        timer->start(maj);
        break;
    case 'M':
        maj = maj + 20;

        timer->stop();
        timer->start(maj);
        break;
    case 'X':
        carte ++;
        if(carte > 3)
            carte = 1;
        QString depth (":/heightmap-");
        depth += QString::number(carte) ;
        depth += ".png" ;

        loadMap(depth);
        break;



    }

}


void TriangleWindow::displayPoints()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    uint id = 0;
    for(int i = 0; i < m_image.width(); i++)
    {
        for(int j = 0; j < m_image.height(); j++)
        {
            id = i*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

        }
    }
    glEnd();
}


void TriangleWindow::displayTriangles()
{
    if (season==0) glColor3f(0.5f, 0.8f, 0.5f);
    else if (season==1) glColor3f(0.8f,0.5f, 0.5f);
    else if (season==2) glColor3f(0.5f, 0.5f, 0.8f);
    else if (season==3) glColor3f(1.0f, 1.0f, 1.0f);


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

void TriangleWindow::displayTrianglesC()
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


void TriangleWindow::displayLines()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
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
            id = i*m_image.width() +j;
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

            id = (i+1)*m_image.width() +j+1;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = (i+1)*m_image.width() +(j);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
        }
    }

    glEnd();
}

void TriangleWindow::displayTrianglesTexture()
{
    // glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    uint id = 0;

    for(int i = 0; i < m_image.width()-1; i++)
    {
        for(int j = 0; j < m_image.height()-1; j++)
        {

            id = i*m_image.width() +j;
            //displayColor(p[id].z);
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
            id = (i+1)*m_image.width() +j;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
        }
    }
    glEnd();
}


void TriangleWindow::displayColor(float alt)
{

    if (alt > 0.2)
    {
        //glColor3f(01.0f, 1.0f, 1.0f);
        glTexCoord2i(1,0);
    }
    else     if (alt > 0.1)
    {
        //glColor3f(alt, 1.0f, 1.0f);
        glTexCoord2i(1,0);
    }
    else     if (alt > 0.05f)
    {
        //glColor3f(01.0f, alt, alt);
        glTexCoord2i(1,0);
    }
    else
    {
        //glColor3f(0.0f, 0.0f, 1.0f);
        glTexCoord2i(1,0);
    }

}


void TriangleWindow::updateParticlesAut()
{
    int id2;
#pragma omp parallel
    {
#pragma omp for
        for(int id = 0; id < numParticules; id++)
        {
            particules[id].z -= 0.0003f * ((float) minP + (rand() % (int)(maxP - minP + 1)));
            id2 = (particules[id].x)*m_image.width() + particules[id].y;
            if(id2<0){
                id2 = 0;
            }
            if(particules[id].z < p[id2].z)
            {
                int angle =minP + (rand() % (int)(maxP - minP + 1));
                int dist = (rand() % (int)(100 ));
                int alt = (rand() % (int)(100));
                float x = sin(
                            ((3.14159 * 2) *
                             angle
                             )/360
                            )*dist;
                float y = cos(
                            ((3.14159 * 2) *
                             angle
                             )/360
                            )*dist;

                particules[id].x = 0.5f;
                particules[id].y = 0.5f;
                particules[id].z = (float)(alt)/100;

            }
        }
    }

    glColor3f(0.2f, 0.2f, 1.0f);
    glPointSize(0.01f);
    glBegin(GL_POINTS);
    for(int id = 0; id < numParticules; id++)
    {
        glVertex3f(
                    particules[id].x,
                    particules[id].y,
                    particules[id].z);


    }
    glEnd();
}

void TriangleWindow::updateParticlesHiv()
{
    int id2;
#pragma omp parallel
    {
#pragma omp for
        for(int id = 0; id < numParticules; id++)
        {
            particules[id].z -= 0.00001f * ((float) minP + (rand() % (int)(maxP - minP + 1)));
            id2 = (particules[id].x)*m_image.width() + particules[id].y;
            if(id2 < 0){
                id2 =0;
            }
            if(particules[id].z < p[id2].z)
            {
                int angle =minP + (rand() % (int)(maxP - minP + 1));
                int dist = (rand() % (int)(100 ));
                int alt = (rand() % (int)(100));
                float x = sin(
                            ((3.14159 * 2) *
                             angle
                             )/360
                            )*dist;
                float y = cos(
                            ((3.14159 * 2) *
                             angle
                             )/360
                            )*dist;

                particules[id].x = 0.5f;
                particules[id].y = 0.5f;
                particules[id].z = (float)(alt)/100;
            }
        }
    }
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(0.0001f);
    glBegin(GL_POINTS);
    for(int id = 0; id < numParticules; id++)
    {
        glVertex3f(
                    particules[id].x,
                    particules[id].y,
                    particules[id].z);


    }
    glEnd();
}

/*
 * Format de sauvegarde :  anim;etat;rotX;rotY;ss
 */
QString paramCamera::toSave()
{
    QString st="";
    st+=QString::number(this->anim);
    st+=";";
    st+=QString::number(this->etat);
    st+=";";
    st+=QString::number(this->rotX);
    st+=";";
    st+=QString::number(this->rotY);
    st+=";";
    st+=QString::number(this->ss);

    return st;

}

void paramCamera::loadParam(float anim, int etat, float rotX, float rotY, float ss)
{
    this->anim=anim;
    this->etat=etat;
    this->rotX = rotX;
    this->rotY = rotY;
    this->ss = ss;

}
