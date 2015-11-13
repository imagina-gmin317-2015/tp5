#include "objetply3d.h"

#include <QDebug>
#include <QStringList>
#include <QFile>
#include <QTextStream>

//essaie render

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>


ObjetPly3D::ObjetPly3D(QString pathToTheFile)
{

    //qDebug() << "Lecture du fichier en cours";

    m_pathToTheFile = pathToTheFile;

    QFile file (pathToTheFile);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
       qDebug() << "impossible de lire le fichier ";// << pathToTheFile;
       return;
    }

    QTextStream flux(&file);
    flux.setCodec("UTF-8");

    QString ligne;

    //les 3 premiere ligne ne sont pas utile
    for(int i = 0; i < 3; ++i)
        ligne = flux.readLine();

    //la 4 eme ligne contient le nombre de points
    ligne = flux.readLine();
    QStringList strList = ligne.split(" ");
    m_nbPoints = strList[2].toInt();
    m_points = new float[m_nbPoints*3];


    //les 6 ligne d'apres sont inutiles
    for(int i = 0; i < 6; ++i)
        ligne = flux.readLine();

    //la 11 eme ligne contient le nombre de face
    ligne = flux.readLine();
    strList = ligne.split(" ");
    m_nbFace = strList[2].toInt();

    //encore 2 ligne inutile
    for(int i = 0; i < 2; ++i)
        ligne = flux.readLine();

    //maitnenant on peu lire tout les points
    //qDebug() << "Boucle de lecture des points";
    for(int i = 0; i < m_nbPoints; ++i){
        ligne = flux.readLine();
        strList = ligne.split(" ");

        m_points[i*3] = strList[0].toFloat();
        m_points[i*3+1] = strList[1].toFloat();
        m_points[i*3+2] = strList[2].toFloat();
    }
    //qDebug() << "Fin de lecture des points";

    //ensuite on lit toutes les faces
    m_faces = new int [m_nbFace*4];
    int numPtsParFace;
    //qDebug() << "Boucle de lecture des faces";
    for(int i = 0; i < m_nbFace; ++i){

        ligne = flux.readLine();
        strList = ligne.split(" ");

        numPtsParFace = strList[0].toInt();

        for(int j = 0; j < numPtsParFace ; ++j)
            m_faces[i*4+j] = strList[j+1].toInt();

        //si la face est un triangle alors le quatrième point n'existe pas, on le met a -1
        if(numPtsParFace == 3)
            m_faces[i*4+3] = -1;
    }

    //qDebug() << "Fin de lecture";
}

ObjetPly3D::ObjetPly3D()
{

}

void ObjetPly3D::renderPoints(float posX, float posY, float posZ, float scale){

    //qDebug() << "Debut render points";

    int numPts;

    glBegin(GL_POINTS);

    for(int i = 0; i < m_nbFace*4; ++i){

        numPts = m_faces[i];
        if(numPts != -1)
            glVertex3f(posX+m_points[numPts*3]*scale, posY+m_points[numPts*3+1]*scale, posZ+m_points[numPts*3+2]*scale);

    }
    glEnd();

    //qDebug() << "Fin render points";

}

void ObjetPly3D::renderFace(float posX, float posY, float posZ, float scale){

    //qDebug() << "Debut render face";

    int numPts;
    int cpt;

    for(int i = 0; i < m_nbFace; ++i){

        if(m_faces[i*4+3] == -1){
            glBegin(GL_TRIANGLES);
            cpt = 3;
        }
        else{//m_faces[i*4+1] != -1
            glBegin(GL_QUADS);
            cpt = 4;
        }

        for(int j = 0; j < cpt; ++j){
            numPts = m_faces[i*4+j];
            glVertex3f(posX+m_points[numPts*3]*scale, posY+m_points[numPts*3+1]*scale, posZ+m_points[numPts*3+2]*scale);
        }

        glEnd();
    }

    //qDebug() << "Fin render face";

}


QString ObjetPly3D::pathToTheFile() const
{
    return m_pathToTheFile;
}

void ObjetPly3D::setPathToTheFile(const QString &pathToTheFile)
{
    m_pathToTheFile = pathToTheFile;
}

float *ObjetPly3D::points() const
{
    return m_points;
}

void ObjetPly3D::setPoints(float *points)
{
    m_points = points;
}

int *ObjetPly3D::faces() const
{
    return m_faces;
}

void ObjetPly3D::setFaces(int *faces)
{
    m_faces = faces;
}

int ObjetPly3D::nbPoints() const
{
    return m_nbPoints;
}

void ObjetPly3D::setNbPoints(int nbPoints)
{
    m_nbPoints = nbPoints;
}

int ObjetPly3D::nbFace() const
{
    return m_nbFace;
}

void ObjetPly3D::setNbFace(int nbFace)
{
    m_nbFace = nbFace;
}

float ObjetPly3D::posX() const
{
    return m_posX;
}

void ObjetPly3D::setPosX(float posX)
{
    m_posX = posX;
}

float ObjetPly3D::posY() const
{
    return m_posY;
}

void ObjetPly3D::setPosY(float posY)
{
    m_posY = posY;
}

float ObjetPly3D::posZ() const
{
    return m_posZ;
}

void ObjetPly3D::setPosZ(float posZ)
{
    m_posZ = posZ;
}


