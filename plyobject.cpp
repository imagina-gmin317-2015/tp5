#include "plyobject.h"

PlyObject::PlyObject(QString filename)
    :m_filename(filename)
{
    readPlyFile();
}

PlyObject::PlyObject()
{
}

void PlyObject::readPlyFile()
{
    QString fic = "..\\tp5\\" + m_filename;
    QFile file(fic);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "impossible d'ouvrir le fichier " << fic;
        return;
    }

    qDebug() << "file open :" << fic;

    QTextStream flux(&file);

    QString ligne;
    flux.readLine();//On saute les lignes intulies
    flux.readLine();
    flux.readLine();
    ligne = flux.readLine();

    QStringList parsedLigne = ligne.split(" ");
    m_nbVertex = parsedLigne[2].toInt();

    ligne = flux.readLine();//On saute les lignes intulies
    parsedLigne = ligne.split(" ");
    /*flux.readLine();
    flux.readLine();
    flux.readLine();
    flux.readLine();
    flux.readLine();*/
    while (parsedLigne[0].compare("element") != 0)
    {
        ligne = flux.readLine();
        parsedLigne = ligne.split(" ");
    }
    //ligne = flux.readLine();
    //parsedLigne = ligne.split(" ");
    m_nbFaces = parsedLigne[2].toInt();
    //qDebug() << "nb Vertex :" << m_nbVertex << " nbFaces " << m_nbFaces;

    flux.readLine();//On saute les lignes intulies
    flux.readLine();
    m_tabPoints = new float[m_nbVertex * 3];

    int i = 0;
    int j = 0;

    /*Lecture des coordonnees de chaque point*/
    while(!flux.atEnd() && i != m_nbVertex)
    {
        ligne = flux.readLine();
        parsedLigne = ligne.split(" ");

        m_tabPoints[j] = parsedLigne[0].toFloat();
        m_tabPoints[j+1] = parsedLigne[1].toFloat();
        m_tabPoints[j+2] = parsedLigne[2].toFloat();
        j += 3;
        ++i;
    }
    //qDebug() << m_tabPoints[j-3] << m_tabPoints[j-2] << m_tabPoints[j-1];

    i = 0;
    j = 0;

    m_tabFaces = new int[m_nbFaces * 4];
    /*Lecture des coordonnees de chaque point*/
    while(!flux.atEnd() && i != m_nbFaces)
    {
        ligne = flux.readLine();
        parsedLigne = ligne.split(" ");

        m_tabFaces[j] = parsedLigne[1].toInt();
        m_tabFaces[j+1] = parsedLigne[2].toInt();
        m_tabFaces[j+2] = parsedLigne[3].toInt();

        if(parsedLigne[0].toInt() != 3)
            m_tabFaces[j+3] = parsedLigne[4].toInt();//Face carres
        else
            m_tabFaces[j+3] = -1;//Face triangle

        j += 4;

        ++i;
    }

    //qDebug() << m_tabFaces[j-4] << m_tabFaces[j-3] << m_tabFaces[j-2] << m_tabFaces[j-1];
}

float* PlyObject::getTabPoints() const
{
    return m_tabPoints;
}

int* PlyObject::getTabFaces() const
{
    return m_tabFaces;
}

int PlyObject::getNbFaces() const
{
    return m_nbFaces;
}

QString PlyObject::getFilename() const
{
    return m_filename;
}

float PlyObject::getAngle() const
{
    return m_angle;
}

void PlyObject::setAngle(float angle)
{
    m_angle = angle;
}
