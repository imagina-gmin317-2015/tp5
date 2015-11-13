#include "filemanager.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

FileManager::FileManager(GameWindow* game)
{
    this->m_fileName = game->title();
    this->m_carte = game->getCarte();
    this->m_day = game->getDay();
    this->m_etat = game->getCamera()->getEtat();
    this->m_rotX = game->getCamera()->getRotX();
    this->m_rotY = game->getCamera()->getRotY();
    this->m_ss = game->getCamera()->getScale();
}

void FileManager::saveData()
{
    QString fic = "..\\tp5\\save\\" + m_fileName + ".ply";
    QFile file(fic);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream flux(&file);

    flux.setCodec("UTF-8");

    flux << m_carte << endl
         << m_day << endl
         << m_etat << endl
         << m_rotX << endl
         << m_rotY << endl
         << m_ss;

    qDebug() << "SAVED !!" << fic;
}

void FileManager::readData()
{
    QString fic = "..\\tp5\\save\\" + m_fileName + ".ply";
    QFile file(fic);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream flux(&file);

    QString ligne;

    ligne = flux.readLine();
    m_carte = ligne.toInt();
    ligne = flux.readLine();
    m_day = ligne.toInt();
    ligne = flux.readLine();
    m_etat = ligne.toInt();
    ligne = flux.readLine();
    m_rotX = ligne.toFloat();
    ligne = flux.readLine();
    m_rotY = ligne.toFloat();
    ligne = flux.readLine();
    m_ss = ligne.toFloat();

    qDebug() << "LOAD !!";
}

void FileManager::save3dObjects(PlyObject* tab3dObjects, int* tabNbArbresParType, int sizeOfTab3d, float* tabPos,
                                float* tabTaille, int nbArbreTotal, int* tabColor, float* tabRotation)
{
    if(sizeOfTab3d > 0)
    {
        QString fic = "..\\tp5\\save\\" + m_fileName + ".ply";
        QFile file(fic);

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
            return;

        QTextStream flux(&file);

        flux.setCodec("UTF-8");

        flux << endl << sizeOfTab3d << endl
             << nbArbreTotal << endl;

        int k = 0;
        for(int i = 0; i < sizeOfTab3d; ++i)
        {
            flux << tab3dObjects[i].getFilename() << endl
                 << tabNbArbresParType[i] << endl;

            for(int j = 0; j < tabNbArbresParType[i]; ++j)
            {
                flux << tabPos[k] << " "
                     << tabPos[k+1] << " "
                     << tabPos[k+2] << " "
                     << tabTaille[k/3] << " "
                     << tabColor[k] << " "
                     << tabColor[k+1] << " "
                     << tabColor[k+2] << " "
                     << tabRotation[k/3] << endl;

                k+=3;
            }
        }

        qDebug() << "3D OBJECTS SAVED !!" << fic;
    }
}

PlyObject* FileManager::read3dObjects()
{
    QString fic = "..\\tp5\\save\\" + m_fileName + ".ply";
    QFile file(fic);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return NULL;

    QTextStream flux(&file);

    QString ligne;
    for(int i = 0; i < 6; ++i)
        flux.readLine();

    ligne = flux.readLine();
    m_sizeOfTab3d = ligne.toInt();
    ligne = flux.readLine();
    m_nb3dObject = ligne.toInt();

    m_tab3dObjects = new PlyObject[m_sizeOfTab3d];
    m_tabArbresParType = new int[m_sizeOfTab3d];
    m_tabPos = new float[3*m_nb3dObject];
    m_tabColor = new int[3*m_nb3dObject];
    m_tabTaille = new float[m_nb3dObject];
    m_tabRotation = new float[m_nb3dObject];

    QStringList parsedLigne;

    qDebug() << m_nb3dObject << " tab3dSize : " << m_sizeOfTab3d << endl;
    int k = 0;

    for(int i = 0; i < m_sizeOfTab3d; ++i)
    {
        ligne = flux.readLine();
        m_tab3dObjects[i] = PlyObject(ligne);
        qDebug() << ligne;
        ligne = flux.readLine();
        m_tabArbresParType[i] = ligne.toInt();

//        qDebug() << m_tab3dObjects[i].getFilename() << " "
//                 << m_tabArbresParType[i] << " ";

        for(int j = 0; j < m_tabArbresParType[i]; ++j)
        {
            ligne = flux.readLine();
            parsedLigne = ligne.split(" ");
            m_tabPos[k]     = parsedLigne[0].toFloat();
            m_tabPos[k+1]   = parsedLigne[1].toFloat();
            m_tabPos[k+2]   = parsedLigne[2].toFloat();
            m_tabTaille[k/3]= parsedLigne[3].toFloat();
            m_tabColor[k]     = parsedLigne[4].toInt();
            m_tabColor[k+1]   = parsedLigne[5].toInt();
            m_tabColor[k+2]   = parsedLigne[6].toInt();
            m_tabRotation[k/3] = parsedLigne[7].toFloat();

            k += 3;
        }
    }

    return NULL;
}

int FileManager::carte() const
{
    return m_carte;
}

void FileManager::setCarte(int carte)
{
    m_carte = carte;
}

int FileManager::day() const
{
    return m_day;
}

void FileManager::setDay(int day)
{
    m_day = day;
}

int FileManager::etat() const
{
    return m_etat;
}

void FileManager::setEtat(int etat)
{
    m_etat = etat;
}

float FileManager::rotX() const
{
    return m_rotX;
}

void FileManager::setRotX(float rotX)
{
    m_rotX = rotX;
}

float FileManager::rotY() const
{
    return m_rotY;
}

void FileManager::setRotY(float rotY)
{
    m_rotY = rotY;
}

float FileManager::ss() const
{
    return m_ss;
}

void FileManager::setSs(float ss)
{
    m_ss = ss;
}

float *FileManager::getTabPos() const
{
    return m_tabPos;
}

float *FileManager::getTabTaille() const
{
    return m_tabTaille;
}

int *FileManager::getTabColor() const
{
    return m_tabColor;
}

float *FileManager::getTabRotation() const
{
    return m_tabRotation;
}

int FileManager::getNb3dObject() const
{
    return m_nb3dObject;
}

PlyObject *FileManager::getTab3dObjects() const
{
    return m_tab3dObjects;
}

int *FileManager::getTabArbresParType() const
{
    return m_tabArbresParType;
}

int FileManager::getSizeOfTab3d() const
{
    return m_sizeOfTab3d;
}

